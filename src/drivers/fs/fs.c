#include "include/drivers/fs/fs.h"
#include "include/drivers/ata.h"
#include "lib/string.h"
#include "vga.h"
#include <stdint.h>

typedef struct
{
    char magic[8];               // "God_spud" // HI @PORK 1271013498406830126 on discord
    unsigned short version;      // 1 //2 BYTES
    unsigned char reserved[502]; // FILL UP REST so 512-majic-version(bytesizes)
} __attribute__((packed)) fs_header_t;

typedef struct
{
    char filename[255];     // filename can be up to 255char long(ASCII)
    char extension[15];     // file extension can be up to 15char long(ASCII)
    unsigned int flags;     // 4 bytes def later
    uint64_t size;          // 64 bytes
    uint64_t start_sector;  // 64 bytes start pos end pos from start_pos-size
    uint64_t lenght;        // 64 bytes no need to recaclulate file size from start and end pos everytime
    unsigned char data[46]; // FILL UP REST so 512-bytes //maybe shd let u store 226 bytes of data here uk save space
} __attribute__((packed)) fs_entry_t;

typedef struct
{
    char filename[255];
    char extension[15];

} file_t;

void fs_init(void)
{
    fs_header_t header;
    memset(&header, 0, sizeof(header));
    memcpy(header.magic, "God_spud", sizeof(header.magic));
    for (int counter = 0; counter < (int)sizeof(header.reserved); counter++)
    {
        header.reserved[counter] = '\0';
    }
    header.version = 1;
    if (ata_write_sector(0, &header))
    {
        return;
    }
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

    // Write file data to disk starting at the specified sector
    // unsigned int sectors_needed = (size + 511) / 512; // Calculate how many sectors are needed
    // for (unsigned int i = 0; i < sectors_needed; i++)
    //{
    //    unsigned int offset = i * 512;
    //    unsigned int bytes_to_write = (size - offset > 512) ? 512 : (size - offset);
    //    if (ata_write_sector(entry.start_sector, (const char *)data + offset))
    //    {
    //        return -1; // Error writing file data
    //    }
    //}

    ata_write_sector(1, &entry);

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