#include "include/drivers/fs/fs.h"
#include "include/drivers/ata.h"
#include "lib/string.h"
#include "vga.h"

typedef struct
{
    char magic[8];               // "God_spud" // HI @PORK 1271013498406830126 on discord
    unsigned short version;      // 1 //2 BYTES
    unsigned char reserved[491]; // FILL UP REST so 512-majic-version(bytesizes)
} __attribute__((packed)) fs_header_t;

typedef struct
{
    char filename[255];          // filename can be up to 255char long(ASCII)
    char extension[15];          // file extension can be up to 15char long(ASCII)
    unsigned int flags;          // 4 bytes def later
    unsigned int size;           // 4 bytes
    unsigned int start_sector;   // 4 bytes start pos end pos from start_pos-size
    unsigned int lenght;         // 4 bytes no need to recaclulate file size from start and end pos everytime
    unsigned char reserved[226]; // FILL UP REST so 512-bytes
} __attribute__((packed)) fs_entry_t;

void fs_init(void)
{
    fs_header_t header;
    memset(&header, 0, sizeof(header));
    memcpy(header.magic, "God_spud", sizeof(header.magic));
    header.version = 1;
    if (ata_write_sector(0, &header))
    {
        return;
    }
}

int fs_create_file(const char *filename, const char *extension, const void *data, unsigned int size)
{
    fs_entry_t entry;
    memset(&entry, 0, sizeof(entry));
    strncpy(entry.filename, filename, sizeof(entry.filename) - 1);
    strncpy(entry.extension, extension, sizeof(entry.extension) - 1);
    entry.size = size;
    entry.start_sector = 1;                            // For simplicity, we start at sector 1 (after header)
    entry.lenght = size;                               // In a real implementation, we would need to calculate this based on the data size and sector size
    entry.flags = 0;                                   // No special flags for now
    memset(entry.reserved, 0, sizeof(entry.reserved)); // Reserved bytes set to 0

    // Write file data to disk starting at the specified sector
    unsigned int sectors_needed = (size + 511) / 512; // Calculate how many sectors are needed
    for (unsigned int i = 0; i < sectors_needed; i++)
    {
        unsigned int offset = i * 512;
        unsigned int bytes_to_write = (size - offset > 512) ? 512 : (size - offset);
        if (ata_write_sector(entry.start_sector, (const char *)data + offset))
        {
            return -1; // Error writing file data
        }
    }

    // In a real implementation, we would also need to write the file entry to a directory structure on disk
    return 0; // Success
}
