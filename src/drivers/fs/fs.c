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
    ata_read_sector(0, fs_header);
    if (memcmp(fs_header->magic, "GOSFSG_s", 8) == 0)
        return; // already initialised, don't wipe

    unsigned int disk_size = get_disk_size();
    memset(fs_header, 0, sizeof(fs_super_super_block_t));
    memcpy(fs_header->magic, "GOSFSG_s", sizeof(fs_header->magic));
    uint32_t blocks = disk_size / 511;
    if (blocks == 0)
        blocks = 1;
    fs_header->no_of_super_blocks = blocks;
    fs_header->version = 1;
    ata_write_sector(0, fs_header);

    fs_super_block_t super_block;
    memset(&super_block, 0, sizeof(super_block));
    super_block.type[0] = FS_SECTOR_SUPERBLOCK; // reserve slot 0
    fs_set_super_sector_inuse(1);
    ata_write_sector(1, &super_block);
}

void fs_set_super_sector_inuse(int used)
{
    fs_header->super_blocks_used = used;
}

void fs_set_sector_type(fs_super_block_t *super_block, unsigned int sector_index, uint8_t type)
{
    if (sector_index >= 512)
        return;
    super_block->type[sector_index] = type;
}

uint8_t fs_get_sector_type(fs_super_block_t *super_block, unsigned int sector_index)
{
    if (sector_index >= 512)
        return FS_SECTOR_FREE;
    return super_block->type[sector_index];
}

int fs_is_sector_free(fs_super_block_t *super_block, unsigned int sector_index)
{
    return fs_get_sector_type(super_block, sector_index) == FS_SECTOR_FREE;
}

int fs_superblock_to_sector(int block_no)
{
    return (block_no * 512) + 1;
}

int fs_sector_to_superblock(int sector_no)
{
    return (sector_no - 1) / 512;
}

// returns global sector number of a free sector, -1 if full
// also sets the type in the superblock and writes it back
int fs_find_free_sector()
{
    fs_super_super_block_t buf = {0};
    ata_read_sector(0, &buf);
    int size_of_disk = buf.no_of_super_blocks;

    for (int j = 0; j < size_of_disk; j++)
    {
        fs_super_block_t sb = {0};
        ata_read_sector(fs_superblock_to_sector(j), &sb);
        for (int i = 1; i < 512; i++)
        {
            if (fs_is_sector_free(&sb, i))
            {
                ata_write_sector(fs_superblock_to_sector(j)+i, "HWUIDHIYWHD"); // write back superblock with updated type
                return fs_superblock_to_sector(j) + i;
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
    return file;
}

int fs_create_file(const char *filename, const char *extension,const char *datatype, const char *data)
{
    if (!filename || !extension || !data)
    {
        return -4;
    }
    if (strlen(filename) > 254)
    {
        return -2;
    }
    if (strlen(extension) > 14)
    {
        return -3;
    }

    fs_entry_t FILE ={0};
    memcpy(FILE.filename, filename, strlen(filename));
    memcpy(FILE.extension, extension, strlen(extension));
    
    fs_set_executable(FILE, 0);
    fs_set_symlink(FILE, 0);
    fs_set_hidden(FILE, 0);

    if (datatype == "executable")
    {
        fs_set_executable(FILE, 1);
        fs_set_symlink(FILE, 0);
        fs_set_hidden(FILE, 0);
    }
    else if (datatype == "symlink")
    {
        fs_set_executable(FILE, 0);
        fs_set_symlink(FILE, 1);
        fs_set_hidden(FILE, 0);
    }
    else if (datatype == "hidden")
    {
        fs_set_executable(FILE, 0);
        fs_set_symlink(FILE, 0);
        fs_set_hidden(FILE, 1);
    }
    else
    {
        return -5; // invalid datatype
    }

    unsigned int datasize = strlen(data);
    if (datasize > 512 * 512)
    {
    // use extent
        ;
    }
    else if (datasize > FS_FILE_DATA_SIZE)
    {
    // dont use extent but use length
        ;
    }
    else if (datasize <= FS_FILE_DATA_SIZE)
    {
    // store in 1 sector
        ;
    }
    // unexpected error
    return 1;
}

char *fs_read_file(const char *filename, const char *extension)
{
    ;
}