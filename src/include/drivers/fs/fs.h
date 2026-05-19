#ifndef FS_H
#define FS_H
#include <stdint.h>

typedef struct
{
    char magic[8]; // "GOSFSG_s" // HI @PORK 1271013498406830126 on discord
    // HI @AlexXela 1330929586824937682 on discord
    unsigned short version;            // 1 //2 BYTES
    unsigned short no_of_super_blocks; // 2 BYTES so max is abt 60k
    // Number of super blocks each super block is 512 512 byte secotrs so 262144 bytes total
    uint8_t super_blocks_used;
    unsigned char reserved[499]; // 512-size
} __attribute__((packed)) fs_super_super_block_t;

typedef struct
{
    uint8_t used[512]; // 512 bytes for bitmap (4096 bits, can track 512 sectors and 7 bits for metadata) - each bit represents a sector's usage
} __attribute__((packed)) fs_super_block_t;

typedef struct
{
    char filename[255];        // filename can be up to 255char long(ASCII)
    char extension[15];        // file extension can be up to 15char long(ASCII)
    uint8_t flags;             // 1 byte def later
    unsigned char encoding[8]; // 8 bytes for encoding type (e.g., UTF-8, ASCII) - for future use
    uint64_t size;             // 8 bytes (64-bit unsigned integer)
    uint64_t start_sector;     // 8 bytes (64-bit sector number)
    uint64_t lenght;           // 8 bytes (64-bit file length)
    unsigned char data[206];   // FILL UP REST so 512-bytes total
} __attribute__((packed)) fs_entry_t;

extern fs_super_super_block_t *fs_header;

void fs_init(void);
void fs_set_sector_inuse(fs_super_block_t *super_block, unsigned int sector_index, int in_use);
int fs_is_sector_free(fs_super_block_t *super_block, unsigned int sector_index);
void fs_set_super_sector_inuse(int used);
int fs_find_free_sector();
int fs_create_file(const char *filename, const char *extension, const char *data);
char *fs_read_file(const char *filename, const char *extension);

#endif