#include "include/drivers/fs/fs.h"
#include "include/drivers/ata.h"
#include "lib/string.h"
#include "vga.h"
#include <stdint.h>
#include "magics.h"

static fs_super_super_block_t fs_header_data;
fs_super_super_block_t *fs_header = &fs_header_data;

void fs_init(void)
{
    if (sizeof(fs_super_super_block_t) > 512 || sizeof(fs_super_super_block_t) < 512)
    {
        print_string("FS SUPER SUPER BLOCK TOO BIG OR SMALL\n", COLOR_LIGHT_RED);
        print_string("Size:\n", COLOR_LIGHT_RED);
        char buffer[10] = {0};
        int_to_str(sizeof(fs_super_super_block_t), buffer);
        print_string(buffer, COLOR_LIGHT_RED);
    }
    if (sizeof(fs_super_block_t) > 512 || sizeof(fs_super_block_t) < 512)
    {
        print_string("FS SUPER BLOCK TOO BIG OR SMALL\n", COLOR_LIGHT_RED);
        print_string("Size:\n", COLOR_LIGHT_RED);
        char buffer[10] = {0};
        int_to_str(sizeof(fs_super_super_block_t), buffer);
        print_string(buffer, COLOR_LIGHT_RED);
    }
    if (sizeof(fs_entry_t) > 512 || sizeof(fs_entry_t) < 512)
    {
        print_string("FS ENTRY TOO BIG OR SMALL\n", COLOR_LIGHT_RED);
        print_string("Size:\n", COLOR_LIGHT_RED);
        char buffer[10] = {0};
        int_to_str(sizeof(fs_super_super_block_t), buffer);
        print_string(buffer, COLOR_LIGHT_RED);
    }

    int step;
    unsigned int disk_size = get_disk_size();
    memset(fs_header, 0, sizeof(fs_super_super_block_t));
    memcpy(fs_header->magic, "GOSFSG_s", sizeof(fs_header->magic));
    uint32_t blocks = disk_size / 4096;
    if (blocks == 0)
        blocks = 1;
    fs_header->no_of_super_blocks = blocks;
    fs_header->version = 1;

    if (!(ata_write_sector(0, fs_header)))
        step = 1;

    fs_super_block_t super_block;
    memset(&super_block, 0, sizeof(super_block));
    fs_set_sector_inuse(&super_block, 0, 1); // mark sector 0 (superblock itself) as used
    fs_set_super_sector_inuse(1);
    if (!(ata_write_sector(1, &super_block)))
        step = 2;
}

void fs_set_super_sector_inuse(int used)
{
    fs_header->super_blocks_used = used;
}

void fs_set_sector_inuse(fs_super_block_t *super_block, unsigned int sector_index, int in_use)
{
    unsigned int byte = sector_index / 8;
    unsigned int bit = sector_index % 8;
    if (byte >= 512)
        return;
    if (in_use)
        super_block->used[byte] |= (1 << bit);
    else
        super_block->used[byte] &= ~(1 << bit);
}

int fs_is_sector_free(fs_super_block_t *super_block, unsigned int sector_index)
{
    unsigned int byte = sector_index / 8;
    unsigned int bit = sector_index % 8;
    if (byte >= 512)
        return 0;
    return (super_block->used[byte] & (1 << bit)) == 0;
}

int fs_superblock_to_sector(int block_no)
{
    return (block_no * 4096 + 1);
}

int fs_sector_to_superblock(int sector_no)
{
    return (sector_no - 1) / 4096;
}

int fs_find_free_sector()
{
    fs_super_block_t header;

    for (int counter = 0; counter < fs_header->no_of_super_blocks; counter++)
    {
        int sector = fs_superblock_to_sector(counter);
        ata_read_sector(sector, &header);

        for (int counter_1 = 0; counter_1 < 4096; counter_1++)
        {
            if (fs_is_sector_free(&header, counter_1))
            {
                int sector_loc = 1 + (counter * 4096) + counter_1;
                return sector_loc;
            }
        }
    }

    return -1;
}

int fs_split_file(const char *data)
{
    unsigned int data_len = (data != 0) ? (unsigned int)strlen(data) : 0;
    if (data_len <= FS_FILE_DATA_SIZE)
        return 1;
    return 1 + (int)((data_len - FS_FILE_DATA_SIZE + 511) / 512);
}

fs_entry_t fs_set_executable(fs_entry_t file, int executable)
{
    if ((file.flags & FS_DIR_META) || (file.flags & FS_SYMLINK_DIR_META))
        return file;
    if (executable)
        file.flags = file.flags | FS_EXECUTABLE_FILE_META;
    else
        file.flags = file.flags & ~FS_EXECUTABLE_FILE_META;
    return file;
}

fs_entry_t fs_set_symlink(fs_entry_t file, int symlink)
{
    if ((file.flags & FS_FILE_META) || (file.flags & FS_EXECUTABLE_FILE_META))
        return file;
    if (symlink)
        file.flags = file.flags | FS_SYMLINK_DIR_META;
    else
        file.flags = file.flags & ~FS_SYMLINK_DIR_META;
    return file;
}

fs_entry_t fs_set_hidden(fs_entry_t file, int hidden)
{
    if (hidden)
        file.flags = file.flags | FS_HIDDEN_META;
    else
        file.flags = file.flags & ~FS_HIDDEN_META;
    return file;
}

fs_entry_t fs_set_compressed(fs_entry_t file, int compressed)
{
    if (compressed)
        file.flags = file.flags | FS_COMPRESSED_META;
    else
        file.flags = file.flags & ~FS_COMPRESSED_META;
}

int fs_create_file(const char *filename, const char *extension, const char *data)
{
    if (!filename || !extension || !data)
        return -4;
    if (strlen(filename) > 254)
        return -2;
    if (strlen(extension) > 14)
        return -3;

    unsigned int data_len = (unsigned int)strlen(data);

    fs_entry_t entry;
    memset(&entry, 0, sizeof(entry));
    strncpy(entry.filename, filename, sizeof(entry.filename) - 1);
    strncpy(entry.extension, extension, sizeof(entry.extension) - 1);
    entry.size = data_len;
    entry.flags = 0;
    entry.extent_sector = 0;

    fs_super_block_t sb;
    int sb_sector = fs_superblock_to_sector(0);
    ata_read_sector(sb_sector, &sb);

    // find sector for the entry itself
    int entry_sector = fs_find_free_sector();
    if (entry_sector < 0)
        return -1;
    fs_set_sector_inuse(&sb, entry_sector - 1, 1);
    entry.start_sector = entry_sector;

    if (data_len <= FS_FILE_DATA_SIZE)
    {
        // fits entirely in entry
        memcpy(entry.data, data, data_len);
        entry.lenght = 1;
        ata_write_sector(sb_sector, &sb);
        ata_write_sector(entry_sector, &entry);
        return 0;
    }

    // copy first chunk into entry
    memcpy(entry.data, data, FS_FILE_DATA_SIZE);

    // build extent table for remaining data
    unsigned int remaining = data_len - FS_FILE_DATA_SIZE;
    unsigned int sectors_needed = (remaining + 511) / 512;
    entry.lenght = 1 + sectors_needed;

    // allocate extent table sectors as needed
    fs_extent_t extents[32];
    int extent_count = 0;
    int current_extent_sector = -1;
    int prev_extent_sector = -1;
    int first_extent_sector = -1;

    unsigned int written = FS_FILE_DATA_SIZE;

    while (written < data_len)
    {
        // find a contiguous run
        int run_start = -1;
        int run_len = 0;

        for (int i = 1; i < 4096 && run_len == 0; i++)
        {
            if (fs_is_sector_free(&sb, i))
            {
                run_start = i;
                // count contiguous free sectors
                while (run_start + run_len < 4096 && fs_is_sector_free(&sb, run_start + run_len) && (written + run_len * 512) < data_len + 512)
                {
                    run_len++;
                }
            }
        }

        if (run_start < 0)
            return -1; // disk full

        // mark run as used
        for (int i = 0; i < run_len; i++)
            fs_set_sector_inuse(&sb, run_start + i, 1);

        // write data into run
        for (int i = 0; i < run_len && written < data_len; i++)
        {
            char secbuf[512] = {0};
            unsigned int chunk = data_len - written;
            if (chunk > 512)
                chunk = 512;
            memcpy(secbuf, data + written, chunk);
            ata_write_sector(run_start + i, secbuf);
            written += chunk;
        }

        // add extent
        extents[extent_count].start = run_start;
        extents[extent_count].length = run_len;
        extent_count++;

        // if extent table sector is full, flush and chain
        if (extent_count == 32 || written >= data_len)
        {
            // allocate an extent table sector
            int ext_sec = fs_find_free_sector();
            if (ext_sec < 0)
                return -1;
            fs_set_sector_inuse(&sb, ext_sec - 1, 1);

            // if last extent, sentinel = 0,0
            // if chaining, last entry = 0, next_ext_sec
            char ext_buf[512] = {0};
            fs_extent_t *tbl = (fs_extent_t *)ext_buf;

            int entries_to_write = extent_count;
            for (int i = 0; i < entries_to_write; i++)
            {
                tbl[i].start = extents[i].start;
                tbl[i].length = extents[i].length;
            }

            if (written < data_len)
            {
                // chain: sentinel points to next extent sector (filled later)
                tbl[31].start = 0;
                tbl[31].length = 0; // will be filled when we know next sector
            }

            ata_write_sector(ext_sec, ext_buf);

            // link previous extent sector to this one
            if (prev_extent_sector >= 0)
            {
                char prev_buf[512] = {0};
                ata_read_sector(prev_extent_sector, prev_buf);
                fs_extent_t *prev_tbl = (fs_extent_t *)prev_buf;
                prev_tbl[31].start = 0;
                prev_tbl[31].length = ext_sec;
                ata_write_sector(prev_extent_sector, prev_buf);
            }

            if (first_extent_sector < 0)
                first_extent_sector = ext_sec;

            prev_extent_sector = ext_sec;
            extent_count = 0;
        }
    }

    entry.extent_sector = (first_extent_sector >= 0) ? first_extent_sector : 0;
    ata_write_sector(sb_sector, &sb);
    ata_write_sector(entry_sector, &entry);
    return 0;
}

// TODO: later
char *fs_read_file(const char *filename, const char *extension)
{
    static fs_entry_t entry;
    ata_read_sector(1, &entry);

    if (strcmp(entry.filename, filename) == 0 && strcmp(entry.extension, extension) == 0)
    {
        print_string(entry.data, COLOR_CYAN);
        return entry.data;
    }
    else
    {
        print_string("File not found", COLOR_RED);
        return 0;
    }
}
