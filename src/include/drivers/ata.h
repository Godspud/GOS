#ifndef ATA_H
#define ATA_H

#include "io.h"

// === SECONDARY IDE CHANNEL (where disk.img is connected) ===
#define ATA_SEC_DATA 0x170       // Read/write data
#define ATA_SEC_ERROR 0x171      // Read error info
#define ATA_SEC_SECTOR_CNT 0x172 // How many sectors to read/write
#define ATA_SEC_LBA_LOW 0x173    // Low 8 bits of sector address
#define ATA_SEC_LBA_MID 0x174    // Middle 8 bits
#define ATA_SEC_LBA_HIGH 0x175   // High 8 bits
#define ATA_SEC_DRIVE 0x176      // Drive select (master/slave)
#define ATA_SEC_STATUS 0x177     // Read status / write command
#define ATA_SEC_COMMAND 0x177    // Same port, write = command

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

// === FUNCTION PROTOTYPES ===
void ata_init(void);
int ata_read_sector(unsigned int lba, unsigned char *buffer);
int ata_write_sector(unsigned int lba, unsigned char *buffer);

#endif