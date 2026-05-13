# `magics.h`

### VGA

- `VGA_MEMORY` `0xB8000`
- `VGA_WIDTH` `80`
- `VGA_HEIGHT` `25`
- `terminal_height` `24`

### VGA Colors

- `COLOR_BLACK` `0`
- `COLOR_BLUE` `1`
- `COLOR_GREEN` `2`
- `COLOR_CYAN` `3`
- `COLOR_RED` `4`
- `COLOR_MAGENTA` `5`
- `COLOR_BROWN` `6`
- `COLOR_LIGHT_GREY` `7`
- `COLOR_DARK_GREY` `8`
- `COLOR_LIGHT_BLUE` `9`
- `COLOR_LIGHT_GREEN` `10`
- `COLOR_LIGHT_CYAN` `11`
- `COLOR_LIGHT_RED` `12`
- `COLOR_LIGHT_MAGENTA` `13`
- `COLOR_LIGHT_BROWN` `14`
- `COLOR_WHITE` `15`

### ATA IO Base

- `ATA_IO_PRIMARY` `0x1F0`
- `ATA_IO_SECONDARY` `0x170`
- `ATA_IO_BASE` `ATA_IO_PRIMARY`

### ATA Registers

- `ATA_SEC_DATA` `ATA_IO_BASE + 0` Read/write data
- `ATA_SEC_ERROR` `ATA_IO_BASE + 1` Read error info
- `ATA_SEC_SECTOR_CNT` `ATA_IO_BASE + 2` How many sectors to read/write
- `ATA_SEC_LBA_LOW` `ATA_IO_BASE + 3` Low 8 bits of sector address
- `ATA_SEC_LBA_MID` `ATA_IO_BASE + 4` Middle 8 bits
- `ATA_SEC_LBA_HIGH` `ATA_IO_BASE + 5` High 8 bits
- `ATA_SEC_DRIVE` `ATA_IO_BASE + 6` Drive select (master/slave)
- `ATA_SEC_STATUS` `ATA_IO_BASE + 7` Read status
- `ATA_SEC_COMMAND` `ATA_IO_BASE + 7` Write command

### ATA Commands

- `ATA_CMD_READ` `0x20` Read sectors
- `ATA_CMD_WRITE` `0x30` Write sectors

### ATA Status Bits

- `ATA_STATUS_BSY` `0x80` Busy - drive working
- `ATA_STATUS_RDY` `0x40` Ready - drive ready
- `ATA_STATUS_DRQ` `0x08` Data Request - ready for I/O
- `ATA_STATUS_ERR` `0x01` Error occurred

### ATA Drive Select

- `ATA_DRIVE_MASTER` `0xA0` Master drive
- `ATA_DRIVE_SLAVE` `0xB0` Slave drive
- `ATA_DRIVE_LBA` `0x40` Use LBA addressing (not CHS)
- `ATA_TARGET_DRIVE` `ATA_DRIVE_SLAVE`

### Keyboard

- `KEYBOARD_DATA_PORT` `0x60`
- `KEYBOARD_STATUS_PORT` `0x64`
