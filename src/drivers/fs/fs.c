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
    unsigned int sectors_needed = (unsigned int)fs_split_file(data);

    fs_entry_t entry;
    memset(&entry, 0, sizeof(entry));
    strncpy(entry.filename, filename, sizeof(entry.filename) - 1);
    strncpy(entry.extension, extension, sizeof(entry.extension) - 1);
    entry.size = data_len;
    entry.lenght = sectors_needed;
    entry.flags = 0;

    fs_super_block_t header;

    for (int sb = 0; sb < fs_header->no_of_super_blocks; sb++)
    {
        int sb_sector = fs_superblock_to_sector(sb);
        ata_read_sector(sb_sector, &header);

        for (int start = 1; start + (int)sectors_needed <= 4096; start++)
        {
            int run_is_free = 1;
            for (unsigned int i = 0; i < sectors_needed; i++)
            {
                if (!fs_is_sector_free(&header, start + (int)i))
                {
                    run_is_free = 0;
                    break;
                }
            }
            if (!run_is_free)
                continue;

            for (unsigned int i = 0; i < sectors_needed; i++)
                fs_set_sector_inuse(&header, start + (int)i, 1);

            ata_write_sector(sb_sector, &header);

            int global_start = 1 + (sb * 4096) + start;
            entry.start_sector = (uint64_t)global_start;

            {
                unsigned int first_chunk = data_len;
                if (first_chunk > FS_FILE_DATA_SIZE)
                    first_chunk = FS_FILE_DATA_SIZE;
                memcpy(entry.data, data, (int)first_chunk);
                ata_write_sector(global_start, &entry);
            }

            for (unsigned int i = 1; i < sectors_needed; i++)
            {
                char secbuf[512] = {0};
                unsigned int src_off = FS_FILE_DATA_SIZE + (i - 1) * 512;
                unsigned int chunk = data_len - src_off;
                if (chunk > 512)
                    chunk = 512;
                memcpy(secbuf, data + src_off, (int)chunk);
                ata_write_sector(global_start + (int)i, secbuf);
            }

            return 0;
        }
    }

    return -1;
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
