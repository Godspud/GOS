# ata.c

## Dependencies

[include/drivers/ata.h](../include/drivers/ata.h)<br/>
stdint.h(standerd lib)<br/>
[magics.h](../magics.h)

## Functions

### `Min u32(min_u32)`

Type: void<br/>
**Parameters:**

- `unsigned int a`
- `unsigned int b`

**Returns:**

- a
- b
  returns a if b is more than a
  returns b if b is equal to or smaller than a

---

### `ATA Wait BSY(ata_wait_bsy)`

Type: void<br/>
waits till the BSY bit is set

---

### `ATA Wait DRQ(ata_wait_drq)`

Type: void<br/>
waits for the DRQ bit to be set

---

### `ATA init(ata_init)`

Note: INIT func<br/>
Type: void<br/>
Inits ATA for use

---

### `ATA Read Secotr(ata_read_sector)`

Type: int<br/>
Reads a 512byte sector from the disk to \*buffer<br/>
**Parameters:**

- `unsigned int lba`
- `void *buffer`<br/>
  lba used for deciding which 512byte sector to read from<br/>
  buffer is used to write changes to a varable<br/>

**Returns:**

- `-1` -> An ERROR occoured
- `0` -> Success

---

### `ATA Write Sector(ata_write_sector)`

Type: int<br/>
Write a 512byte sector to the disk from \*buffer<br/>
**Parameters:**

- `unsigned int lba`
- `const void *buffer`
  lba is used to decide which 512byte sector to write to<br/>
  buffer is the value to write to the disk<br/>

**Returns:**

- `-1` -> An ERROR occoured
- `0` -> Sucess

---

### `Get Disk Size(get_disk_size)`

Type: int<br/>
Gets the disk size<br/>
**Returns:**

- `-1` -> Drive Error
- `total_sectors` -> total_sectors
