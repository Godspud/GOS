#include "include/drivers/fs/fs.h"
#include "include/drivers/ata.h"
#include <string.h>

typedef struct
{
    char magic[19];              // "#qorkcrastinatingFS" // HI @PORK 1271013498406830126 on discord
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
    memcpy(header.magic, "#qorkcrastinatingFS", sizeof(header.magic));
    header.version = 1;
    if (ata_write_sector(0, &header))
    {
        return;
    }
}
