#include "include/drivers/fs/fs.h"
#include "include/drivers/ata.h"

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
    header.version = 1;
    header.magic[0] = '#';
    header.magic[1] = 'q';
    header.magic[2] = 'o';
    header.magic[3] = 'r';
    header.magic[4] = 'k';
    header.magic[5] = 'c';
    header.magic[6] = 'r';
    header.magic[7] = 'a';
    header.magic[8] = 's';
    header.magic[9] = 't';
    header.magic[10] = 'i';
    header.magic[11] = 'n';
    header.magic[12] = 'a';
    header.magic[13] = 't';
    header.magic[14] = 'i';
    header.magic[15] = 'n';
    header.magic[16] = 'g';
    header.magic[17] = 'F';
    header.magic[18] = 'S';
    for (int i = 19; i < 512; i++)
    {
        header.reserved[i - 19] = 0;
    }
    ata_write_sector(1, (unsigned char *)&header);
}
