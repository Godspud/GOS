#ifndef MAGIC_H
#define MAGIC_H

/**
 * vga magics
 */
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define terminal_height 24

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GREY 7
#define COLOR_DARK_GREY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_LIGHT_BROWN 14
#define COLOR_WHITE 15

/**
 * ata_magics
 */
// === IDE CHANNEL / TARGET SELECTION ===
// QEMU with -drive if=ide,index=1 maps to PRIMARY channel, SLAVE drive.
#define ATA_IO_PRIMARY 0x1F0
#define ATA_IO_SECONDARY 0x170
#define ATA_IO_BASE ATA_IO_PRIMARY

#define ATA_SEC_DATA (ATA_IO_BASE + 0)       // Read/write data
#define ATA_SEC_ERROR (ATA_IO_BASE + 1)      // Read error info
#define ATA_SEC_SECTOR_CNT (ATA_IO_BASE + 2) // How many sectors to read/write
#define ATA_SEC_LBA_LOW (ATA_IO_BASE + 3)    // Low 8 bits of sector address
#define ATA_SEC_LBA_MID (ATA_IO_BASE + 4)    // Middle 8 bits
#define ATA_SEC_LBA_HIGH (ATA_IO_BASE + 5)   // High 8 bits
#define ATA_SEC_DRIVE (ATA_IO_BASE + 6)      // Drive select (master/slave)
#define ATA_SEC_STATUS (ATA_IO_BASE + 7)     // Read status / write command
#define ATA_SEC_COMMAND (ATA_IO_BASE + 7)    // Same port, write = command

// === ATA COMMANDS ===
#define ATA_CMD_READ 0x20  // Read sectors
#define ATA_CMD_WRITE 0x30 // Write sectors

// === STATUS BITS ===
#define ATA_STATUS_BSY 0x80 // Busy - drive working
#define ATA_STATUS_RDY 0x40 // Ready - drive ready
#define ATA_STATUS_DRQ 0x08 // Data Request - ready for I/O
#define ATA_STATUS_ERR 0x01 // Error occurred

// === DRIVE SELECT ===
#define ATA_DRIVE_MASTER 0xA0 // Master drive
#define ATA_DRIVE_SLAVE 0xB0  // Slave drive
#define ATA_DRIVE_LBA 0x40    // Use LBA addressing (not CHS)
#define ATA_TARGET_DRIVE ATA_DRIVE_SLAVE

/**
 * keyboard magics
 */
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

/**
 * fs magics
 */
#define FS_FILE_DATA_SIZE 206

#endif