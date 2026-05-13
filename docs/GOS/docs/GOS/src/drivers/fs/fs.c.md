# `fs.c`

## Dependencies

[include/drivers/fs/fs.h](../../include/drivers/fs/fs.h)<br/>
[include/drivers/ata.h](../../include/drivers/ata.h)<br/>
[lib/string.h](../../lib/string.h)<br/>
[vga.h](../../vga.h)<br/>
stdint.h(standerd lib)<br/>
[magics.h](../../magics.h)<br/>

## Functions

### `FS Init(fs_init)`

Note: INIT func<br/>
Type: void<br/>
Initialises the filesystem, writes the super super block and first super block to disk

---

### `FS Set Super Sector In Use(fs_set_super_sector_inuse)`

Type: void<br/>
Updates the super super block's used count based on how many sectors are in use<br/>
**Parameters:**

- `int used`

---

### `FS Set Sector In Use(fs_set_sector_inuse)`

Type: void<br/>
Marks a sector in the super block bitmap as in use or free<br/>
**Parameters:**

- `fs_super_block_t *super_block`
- `unsigned int sector_index`
- `int in_use`

---

### `FS Is Sector Free(fs_is_sector_free)`

Type: int<br/>
Checks if a sector in the super block bitmap is free<br/>
**Parameters:**

- `fs_super_block_t *super_block`
- `unsigned int sector_index`

**Returns:**

- `1` → sector is free
- `0` → sector is in use

---

### `FS Superblock To Sector(fs_superblock_to_sector)`

Type: int<br/>
Converts a super block number to its corresponding disk sector number<br/>
**Parameters:**

- `int block_no`

**Returns:**

- `(block_no * 512 + 1)` → sector number

---

### `FS Sector To Superblock(fs_sector_to_superblock)`

Type: int<br/>
Converts a disk sector number to its corresponding super block number<br/>
**Parameters:**

- `int sector_no`

**Returns:**

- `(sector_no - 1) / 512` → super block number

---

### `FS Find Free Sector(fs_find_free_sector)`

Type: int<br/>
Scans all super blocks to find the first free sector<br/>

**Returns:**

- `sector_loc` → first free sector
- `-1` → disk full

---

### `FS Create File(fs_create_file)`

Type: int<br/>
Creates a new file entry on disk with the given filename, extension, and data<br/>
**Parameters:**

- `const char *filename`
- `const char *extension`
- `const char *data`
- `unsigned int size`

**Returns:**

- `-2` → filename too big
- `-3` → extension too big
- `0` → success
- `-1` → disk full

---

### `FS Read File(fs_read_file)`

Type: char\*<br/>
Reads a file entry from disk matching the given filename and extension<br/>
**Parameters:**

- `const char *filename`
- `const char *extension`

**Returns:**

- `entry.data` → file data
- `0` → file not found

---

### `FS Open Files(fs_open_files)`

Type: void<br/>
Reserved, not yet implemented
