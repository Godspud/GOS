# `src/drivers/fs/fs.c`

## Dependencies

[include/drivers/fs/fs.h](../../../include/drivers/fs/fs.h\)
[include/drivers/ata.h](../../../include/drivers/ata.h\)
[lib/string.h](../../../lib/string.h\)
[vga.h](vga.h\)
[stdint.h](stdint.h\)
[magics.h](magics.h\)

## Functions

### `void fs_init(void)`

**Parameters:** none

**Returns:** nothing / void

---

### `void fs_set_super_sector_inuse(int used)`

**Parameters:**
- `int used`

**Returns:** nothing / void

---

### `void fs_set_sector_inuse(fs_super_block_t *super_block, unsigned int sector_index, int in_use)`

**Parameters:**
- `fs_super_block_t *super_block`
- `unsigned int sector_index`
- `int in_use`

**Returns:** nothing / void

---

### `int fs_is_sector_free(fs_super_block_t *super_block, unsigned int sector_index)`

**Parameters:**
- `fs_super_block_t *super_block`
- `unsigned int sector_index`

**Returns:**
- `(super_block->used[sector_index] & (0b10000000)) != 1` → Check if the bit is clear (free)

---

### `int fs_superblock_to_sector(int block_no)`

**Parameters:**
- `int block_no`

**Returns:**
- `(block_no * 512 + 1)` → (block_no * 512 + 1)

---

### `int fs_sector_to_superblock(int sector_no)`

**Parameters:**
- `int sector_no`

**Returns:**
- `(sector_no - 1) / 512` → (sector_no - 1) / 512

---

### `int fs_find_free_sector()`

**Parameters:** none

**Returns:**
- `sector_loc` → sector_loc
- `-1` → disk full

---

### `int fs_create_file(const char *filename, const char *extension, const char *data, unsigned int size)`

**Parameters:**
- `const char *filename`
- `const char *extension`
- `const char *data`
- `unsigned int size`

**Returns:**
- `-1` → filename too big
- `-2` → fileext too big
- `0` → 0
- `-1` → disk full

---

### `char *fs_read_file(const char *filename, const char *extension)`

**Parameters:** none

**Returns:**
- `entry.data` → Return just the file data
- `0` → 0

---

### `void fs_open_files()`

**Parameters:** none

**Returns:** nothing / void

