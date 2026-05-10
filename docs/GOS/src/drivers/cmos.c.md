# `cmos.c`

## Dependencies

[include/drivers/cmos.h](../../include/drivers/cmos.h)<br>
[include/drivers/io.h](../../include/drivers/io.h)

## Functions

### `CMOS init(cmos_init)`

Note: INIT func<br>
Type: void<br>
Inits cmos for use

### `CMOS Read(cmos_read)`

Type: unsigned char
Reads the CMOS ports

**Parameters:**

- `unsigned char reg`<br>
  sends the data(reg) to data port 0x70

**Returns:**

- `inb(0x71)` -> data read from the port 0x71

### `BCD to BIN(bcd_to_bin)`

Type: unsigned char<br>
takes a bcd no. and converts it to a bin no.<br>
**Parameters:**

- `unsigned char bcd`<br>
  bcd no to be converted

**Returns:**

- `((bcd & 0x0F) + ((bcd >> 4) * 10))` -> conversion fomula from bcd to bin

### `CMOS Set GMT Offset(cmos_set_gmt_offset)`

TODO: set gmt offset to work with any hh:mm:ss<br>
Type: void<br>
sets the GMT offset to the specified value<br>
**Parameters:**

- `int offset`<br>
  offset to be set to

### `CMOS Get Time`

TODO: set gmt offset to work with any hh:mm:ss<br>
Type: void<br>
gets the time from the cmos chip, thens adds the GMT offsets<br>
**Parameters:**

- `cmos_time *time`<br>
  \*time is used to set the value of the var based on the current time<br>
  Note: cmos_time is a struct from [cmos.h](../../include/drivers/cmos.h)
