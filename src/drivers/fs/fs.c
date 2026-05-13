#include "include/drivers/fs/fs.h"
#include "include/drivers/ata.h"
#include "lib/string.h"
#include "vga.h"
#include <stdint.h>
#include "magics.h"

typedef struct
{
    char filename[255];
    char extension[15];

} file_t;

static fs_super_super_block_t fs_header_data;
fs_super_super_block_t *fs_header = &fs_header_data;

void fs_init(void)
{
    int step;
    fs_super_super_block_t header;
    unsigned int disk_size = get_disk_size();
    unsigned int super_blocks = disk_size / 512;
    print_string("No of super blocks: ", COLOR_LIGHT_CYAN);
    char super_blocks_str[20];
    int_to_str(super_blocks, super_blocks_str);
    print_string(super_blocks_str, COLOR_LIGHT_CYAN);
    print_string("\n", COLOR_LIGHT_CYAN);
    print_string("Disk size: ", COLOR_LIGHT_CYAN);
    char size_str[20];
    int_to_str(disk_size, size_str);
    print_string(size_str, COLOR_LIGHT_CYAN);
    print_string(" sectors(512 bytes)\n", COLOR_LIGHT_CYAN);
    print_string("Disk size: ", COLOR_LIGHT_CYAN);
    char size_str_bytes[20];
    int_to_str(disk_size * 512, size_str_bytes);
    print_string(size_str_bytes, COLOR_LIGHT_CYAN);
    print_string(" bytes\n", COLOR_LIGHT_CYAN);
    memset(&header, 0, sizeof(header));
    memcpy(header.magic, "GOSFSG_s", sizeof(header.magic));
    uint32_t blocks = disk_size / 262144;
    if (blocks == 0)
    {
        blocks = 1;
    }
    header.no_of_super_blocks = blocks;

    for (int counter = 0; counter < (int)sizeof(header.reserved); counter++)
    {
        header.reserved[counter] = '\0';
    }
    header.version = 1;
    memcpy(fs_header, &header, sizeof(header));
    if (!(ata_write_sector(0, &header)))
    {
        step = 1;
    }
    fs_super_block_t super_block;
    memset(&super_block, 0, sizeof(super_block));
    fs_set_sector_inuse(&super_block, 0, 1);
    fs_set_super_sector_inuse(1);
    if (!(ata_write_sector(1, &super_block)))
    {
        step = 2;
    }
}

void fs_set_super_sector_inuse(int used)
{
    /*used is a no of used blocks eg 4 for 4 sectors in a super_block used*/
    fs_header->super_blocks_used = used;
}

void fs_set_sector_inuse(fs_super_block_t *super_block, unsigned int sector_index, int in_use)
{
    if (sector_index >= 512)
    {
        return;
    }
    if (in_use)
    {
        super_block->used[sector_index] |= 0b10000000; // Set the bit to mark as in use
    }
    else
    {
        super_block->used[sector_index] &= ~0b10000000; // Clear the bit to mark as free
    }
}

int fs_is_sector_free(fs_super_block_t *super_block, unsigned int sector_index)
{
    return (super_block->used[sector_index] & 0b10000000) == 0;
}

int fs_superblock_to_sector(int block_no)
{
    return (block_no * 512 + 1);
}

int fs_sector_to_superblock(int sector_no)
{
    return (sector_no - 1) / 512;
}

int fs_find_free_sector()
{
    fs_super_block_t header;

    for (int counter = 0; counter < fs_header->no_of_super_blocks; counter++)
    {
        int sector = fs_superblock_to_sector(counter);

        ata_read_sector(sector, &header);

        for (int counter_1 = 0; counter_1 < 512; counter_1++)
        {
            if (!(header.used[counter_1] & 0b10000000)) // free
            {
                int sector_loc = sector + counter_1;
                return sector_loc;
            }
        }
    }

    return -1; // disk full
}
int fs_split_file(const char *data)
{
    unsigned int data_len = (data != 0) ? (unsigned int)strlen(data) : 0;

    // 1 sector for fs_entry_t (metadata + first 206 bytes)
    if (data_len <= FS_FILE_DATA_SIZE)
        return 1;

    // extra bytes go into full 512-byte sectors
    return 1 + (int)((data_len - FS_FILE_DATA_SIZE + 511) / 512);
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
    entry.size = data_len;         // bytes
    entry.lenght = sectors_needed; // sectors used
    entry.flags = 0;

    fs_super_block_t header;

    for (int sb = 0; sb < fs_header->no_of_super_blocks; sb++)
    {
        int sb_sector = fs_superblock_to_sector(sb);
        ata_read_sector(sb_sector, &header);

        for (int start = 1; start + (int)sectors_needed <= 512; start++)
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

            // reserve all sectors in this run
            for (unsigned int i = 0; i < sectors_needed; i++)
                fs_set_sector_inuse(&header, start + (int)i, 1);

            ata_write_sector(sb_sector, &header);

            int global_start = 1 + (sb * 512) + start;
            entry.start_sector = (uint64_t)global_start;

            // first sector: metadata + first chunk
            {
                unsigned int first_chunk = data_len;
                if (first_chunk > FS_FILE_DATA_SIZE)
                    first_chunk = FS_FILE_DATA_SIZE;
                memcpy(entry.data, data, (int)first_chunk);
                ata_write_sector(global_start, &entry);
            }

            // remaining sectors: raw continuation data
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

    return -1; // disk full or no contiguous run
}

char *fs_read_file(const char *filename, const char *extension)
{
    static fs_entry_t entry;    // Read the file entry structure
    ata_read_sector(1, &entry); // For now, assume file is at sector 1

    // Check if this is the right file (basic check)
    if (strcmp(entry.filename, filename) == 0 && strcmp(entry.extension, extension) == 0)
    {
        print_string(entry.data, COLOR_CYAN); // Print just the file data
        return entry.data;                    // Return just the file data
    }
    else
    {
        print_string("File not found", COLOR_RED);
        return 0;
    }
}

void fs_open_files()
{
}