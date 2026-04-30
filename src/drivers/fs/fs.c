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

int fs_create_file(const char *filename, const char *extension, const char *data, unsigned int size)
{
    fs_entry_t entry;
    memset(&entry, 0, sizeof(entry));
    unsigned int filename_len = strlen(filename);
    for (int counter = 0; counter < (int)filename_len && counter < (int)(sizeof(entry.filename) - 1); counter++)
    {
        entry.filename[counter] = filename[counter];
    }
    for (int counter = 0; counter < (int)sizeof(entry.filename); counter++)
    {
        if (counter >= (int)filename_len)
        {
            entry.filename[counter] = '\0';
        }
    }
    unsigned int extension_len = strlen(extension);
    for (int counter = 0; counter < (int)extension_len && counter < (int)(sizeof(entry.extension) - 1); counter++)
    {
        entry.extension[counter] = extension[counter];
    }
    for (int counter = 0; counter < (int)sizeof(entry.extension); counter++)
    {
        if (counter >= (int)extension_len)
        {
            entry.extension[counter] = '\0';
        }
    }
    // TODO: SET FLAGS AND STUFFS
    unsigned int data_len = strlen(data);
    for (int counter = 0; counter < (int)data_len && counter < (int)(sizeof(entry.data) - 1); counter++)
    {
        entry.data[counter] = data[counter];
    }
    for (int counter = 0; counter < (int)sizeof(entry.data); counter++)
    {
        if (counter >= (int)data_len)
        {
            entry.data[counter] = '\0';
        }
    }
    entry.size = size;
    entry.start_sector = 1; // For simplicity, we start at sector 1 (after header)
    entry.lenght = size;    // In a real implementation, we would need to calculate this based on the data size and sector size
    entry.flags = 0;        // No special flags for now
                            //
    // for (int counter = 0; counter++; counter < 512)
    //{
    //    fs_is_sector_free()
    //}

    // Write file entry to disk - ensure entire sector is initialized
    char sector_buffer[512] = {0};                // Zero entire sector
    memcpy(sector_buffer, &entry, sizeof(entry)); // Copy structure
    ata_write_sector(2, sector_buffer);           // Write clean sector

    return 0; // Success
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