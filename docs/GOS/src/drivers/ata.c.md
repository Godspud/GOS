# ata.c

## Dependencies

[include/drivers/ata.h](../../include/drivers/ata.h)<br>
stdint.h(standerd lib)<br>
[magics.h](../../magics.h)

## Functions

### `static unsigned int min_u32(unsigned int a, unsigned int b)`

**Parameters:**

- `unsigned int a`
- `unsigned int b`

**Returns:**

- `(a < b) ? a : b` → (a < b) ? a : b

---

### `static void ata_wait_bsy(void)`

**Parameters:** none

**Returns:** nothing / void

---

### `static void ata_wait_drq(void)`

**Parameters:** none

**Returns:**

- `void` → void return

---

### `void ata_init(void)`

**Parameters:** none

**Returns:** nothing / void

---

### `int ata_read_sector(unsigned int lba, void *buffer)`

**Parameters:**

- `unsigned int lba`
- `void *buffer`

**Returns:**

- `-1` → -1
- `0` → Success

---

### `int ata_write_sector(unsigned int lba, const void *buffer)`

**Parameters:**

- `unsigned int lba`
- `const void *buffer`

**Returns:**

- `-1` → -1
- `0` → 0

---

### `int get_disk_size()`

**Parameters:** none

**Returns:**

- `-1` → Drive Error
- `total_sectors` → total_sectors
