# `fs.h`

## Dependencies

stdint.h(standerd lib)

## Structs

### FS Super Super Block(`fs_super_super_block_t`)

**Attributes:**

- `packed`

**Fields:**

- `char magic[8]` Magic string `"GOSFSG_s"` used to identify a valid GOSFS disk
- `unsigned short version` Filesystem version number
- `unsigned short no_of_super_blocks` Number of super blocks (each covers 512 sectors of 512 bytes = 262144 bytes)
- `uint8_t super_blocks_used` How many super blocks are currently in use
- `unsigned char reserved[499]` Padding to fill the 512 byte sector

### FS Super Block(`fs_super_block_t`)

**Attributes:**

- `packed`

**Fields:**

- `uint8_t used[512]` Bitmap tracking sector usage — high bit of each byte marks that sector as in use

### FS Entry(`fs_entry_t`)

**Attributes:**

- `packed`

**Fields:**

- `char filename[255]` Filename up to 255 ASCII characters
- `char extension[15]` File extension up to 15 ASCII characters
- `unsigned int flags` File flags (reserved for future use)
- `unsigned char encoding[8]` Encoding type e.g. UTF-8, ASCII (reserved for future use)
- `uint64_t size` File size in bytes
- `uint64_t start_sector` Sector number where the file data begins
- `uint64_t lenght` File length in bytes
- `unsigned char data[206]` Inline file data, fills remainder of the 512 byte sector

## Externs

- `extern fs_super_super_block_t *fs_header`

## Functions

Function declaration for

1. [fs_init](../../../drivers/fs/fs.c#void-fs_initvoid)
2. [fs_set_sector_inuse](../../../drivers/fs/fs.c#void-fs_set_sector_inusefs_super_block_t-super_block-unsigned-int-sector_index-int-in_use)
3. [fs_is_sector_free](../../../drivers/fs/fs.c#int-fs_is_sector_freefs_super_block_t-super_block-unsigned-int-sector_index)
4. [fs_set_super_sector_inuse](../../../drivers/fs/fs.c#void-fs_set_super_sector_inuseint-used)
5. [fs_create_file](../../../drivers/fs/fs.c#int-fs_create_fileconst-char-filename-const-char-extension-const-char-data-unsigned-int-size)
6. [fs_read_file](../../../drivers/fs/fs.c#char-fs_read_fileconst-char-filename-const-char-extension)
