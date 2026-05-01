#include "include/drivers/fs/fs.h"
#include "include/drivers/ata.h"
#include "lib/string.h"
#include "vga.h"
#include <stdint.h>

typedef struct
{
    char filename[255];
    char extension[15];

} file_t;

fs_super_super_block_t *fs_header = 0;

void fs_init(void)
{
    int step;
    fs_super_super_block_t header;
    unsigned int disk_size = get_disk_size();
    unsigned int super_blocks = disk_size / 512;
    print_string("No of super blocks: ", COLOR_LIGHT_CYAN);
    char super_blocks_str[20];
    int_to_str(super_blocks, super_blocks_str, 10);
    print_string(super_blocks_str, COLOR_LIGHT_CYAN);
    print_string("\n", COLOR_LIGHT_CYAN);
    print_string("Disk size: ", COLOR_LIGHT_CYAN);
    char size_str[20];
    int_to_str(disk_size, size_str, 10);
    print_string(size_str, COLOR_LIGHT_CYAN);
    print_string(" sectors(512 bytes)\n", COLOR_LIGHT_CYAN);
    print_string("Disk size: ", COLOR_LIGHT_CYAN);
    char size_str_bytes[20];
    int_to_str(disk_size * 512, size_str_bytes, 10);
    print_string(size_str_bytes, COLOR_LIGHT_CYAN);
    print_string(" bytes\n", COLOR_LIGHT_CYAN);
    memset(&header, 0, sizeof(header));
    memcpy(header.magic, "GOSbyG_s", sizeof(header.magic));
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
    get_disk_size();
}

void fs_set_super_sector_inuse(int used)
{
    /*used is a no of used blocks eg 4 for 4 sectors in a super_block used*/
    fs_header->super_blocks_used = used / fs_header->no_of_super_blocks;
}

void fs_set_sector_inuse(fs_super_block_t *super_block, unsigned int sector_index, int in_use)
{
    if (in_use)
    {
        super_block->bitmap[sector_index] |= 0b10000000; // Set the bit to mark as in use
    }
    else
    {
        super_block->bitmap[sector_index] &= ~0b10000000; // Clear the bit to mark as free
    }
}

int fs_is_sector_free(fs_super_block_t *super_block, unsigned int sector_index)
{

    return (super_block->bitmap[sector_index] & (0b10000000)) != 1; // Check if the bit is clear (free)
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
        int counter = 1 + (counter * 512);

        ata_read_sector(counter, &header);

        for (int i = 0; i < 512; i++)
        {
            if (!(header.bitmap[i] & 0b10000000)) // free
            {
                int sector_loc = 1 + (counter * 512) + i;
                return sector_loc;
            }
        }
    }

    return -1; // disk full
}

int fs_create_file(const char *filename, const char *extension, const char *data, unsigned int size)
{
    fs_entry_t entry;
    memset(&entry, 0, sizeof(entry));

    // copy filename
    strncpy(entry.filename, filename, sizeof(entry.filename) - 1);

    // copy extension
    strncpy(entry.extension, extension, sizeof(entry.extension) - 1);

    // copy data
    strncpy((char *)entry.data, data, sizeof(entry.data) - 1);

    entry.size = size;
    entry.lenght = size;
    entry.flags = 0;

    fs_super_block_t header;

    // loop all superblocks
    for (int counter = 0; counter < fs_header->no_of_super_blocks; counter++)
    {
        int sb_sector = fs_superblock_to_sector(counter);

        ata_read_sector(sb_sector, &header);

        // skip 0 → avoids superblock itself
        for (int counter_1 = 1; counter_1 < 512; counter_1++)
        {
            if (fs_is_sector_free(&header, counter_1))
            {
                // mark used
                fs_set_sector_inuse(&header, counter_1, 1);
                ata_write_sector(sb_sector, &header);

                int global_sector = 1 + (counter * 512) + counter_1;

                entry.start_sector = global_sector;

                // write file
                char buffer[512] = {0};
                memcpy(buffer, &entry, sizeof(entry));

                ata_write_sector(global_sector, buffer);

                return 0;
            }
        }
    }

    return -1; // disk full
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