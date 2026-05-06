# `src/drivers/cmos.c`

## Dependencies

[include/drivers/cmos.h](../../include/drivers/cmos.h\)
[include/drivers/io.h](../../include/drivers/io.h\)

## Functions

### `void cmos_init(void)`

**Parameters:** none

**Returns:** nothing / void

---

### `unsigned char cmos_read(unsigned char reg)`

**Parameters:**
- `unsigned char reg`

**Returns:**
- `inb(0x71)` → inb(0x71)

---

### `static unsigned char bcd_to_bin(unsigned char bcd)`

**Parameters:**
- `unsigned char bcd`

**Returns:**
- `((bcd & 0x0F) + ((bcd >> 4) * 10))` → ((bcd & 0x0F) + ((bcd >> 4) * 10))

---

### `void cmos_set_gmt_offset(int offset)`

**Parameters:**
- `int offset`

**Returns:** nothing / void

---

### `void cmos_get_time(cmos_time *time)`

**Parameters:**
- `cmos_time *time`

**Returns:** nothing / void

