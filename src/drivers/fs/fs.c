#include "include/drivers/fs/fs.h"
#include "include/drivers/ata.h"
#include "lib/string.h"
#include "vga.h"
#include <stdint.h>

// ADD PARTATION SIZE
typedef struct
{
    char magic[8]; // "GOSbyG_s" // HI @PORK 1271013498406830126 on discord
    // HI @AlexXela 1330929586824937682 on discord
    unsigned short version;            // 1 //2 BYTES
    unsigned short no_of_super_blocks; // 2 BYTES so max is abt 60k
    // Number of super blocks each super block is 512 512 byte secotrs so 262144 bytes total
    unsigned char reserved[500]; // FILL UP REST so 512-majic-version(bytesizes)
} __attribute__((packed)) fs_super_super_block_t;

typedef struct
{
    uint8_t bitmap[512]; // 512 bytes for bitmap (4096 bits, can track 512 sectors and 7 bits for metadata) - each bit represents a sector's usage
} __attribute__((packed)) fs_super_block_t;

typedef struct
{
    char filename[255];        // filename can be up to 255char long(ASCII)
    char extension[15];        // file extension can be up to 15char long(ASCII)
    unsigned int flags;        // 4 bytes def later
    unsigned char encoding[8]; // 8 bytes for encoding type (e.g., UTF-8, ASCII) - for future use
    uint64_t size;             // 8 bytes (64-bit unsigned integer)
    uint64_t start_sector;     // 8 bytes (64-bit sector number)
    uint64_t lenght;           // 8 bytes (64-bit file length)
    unsigned char data[206];   // FILL UP REST so 512-bytes total
} __attribute__((packed)) fs_entry_t;

typedef struct
{
    char filename[255];
    char extension[15];

} file_t;

void fs_init(void)
{
    int step;
    fs_super_super_block_t header;
    memset(&header, 0, sizeof(header));
    memcpy(header.magic, "GOSbyG_s", sizeof(header.magic));
    for (int counter = 0; counter < (int)sizeof(header.reserved); counter++)
    {
        header.reserved[counter] = '\0';
    }
    header.version = 1;
    if (ata_write_sector(0, &header))
    {
        step = 1;
    }
    fs_super_block_t super_block;
    fs_set_sector_inuse(&super_block, 1, 1)
        memset(&super_block, 0, sizeof(super_block));
    if (ata_write_sector(1, &super_block))
    {
        step = 2;
    }
}

void fs_set_sector_inuse(fs_super_block_t *super_block, unsigned int sector_index, int in_use)
{
    unsigned int byte_index = sector_index / 8;
    unsigned int bit_index = sector_index % 8;

    if (in_use)
    {
        super_block->bitmap[byte_index] |= (1 << bit_index); // Set the bit to mark as in use
    }
    else
    {
        super_block->bitmap[byte_index] &= ~(1 << bit_index); // Clear the bit to mark as free
    }
}

int fs_is_sector_free(fs_super_block_t *super_block, unsigned int sector_index)
{
    unsigned int byte_index = sector_index / 8;
    unsigned int bit_index = sector_index % 8;

    return (super_block->bitmap[byte_index] & (1 << bit_index)) == 0; // Check if the bit is clear (free)
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

    // Write file entry to disk - ensure entire sector is initialized
    char sector_buffer[512] = {0};                // Zero entire sector
    memcpy(sector_buffer, &entry, sizeof(entry)); // Copy structure
    ata_write_sector(1, sector_buffer);           // Write clean sector

    // In a real implementation, we would also need to write the file entry to a directory structure on disk
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