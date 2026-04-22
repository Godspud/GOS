#include "include/drivers/ata.h"
#include "vga.h"

// Wait until drive is NOT busy
static void ata_wait_bsy(void)
{
    while (inb(ATA_SEC_STATUS) & ATA_STATUS_BSY)
        ; // Keep looping while BSY bit is set
}

// Wait until drive is ready for data transfer
static void ata_wait_drq(void)
{
    while (!(inb(ATA_SEC_STATUS) & ATA_STATUS_DRQ))
        ; // Keep looping until DRQ bit is set
}

// Initialize ATA driver
void ata_init(void)
{
    // Just wait for drive to be ready
    ata_wait_bsy();
    print_string("ATA initialized\n", COLOR_GREEN);
}

// Read 512 bytes from sector at address 'lba'
int ata_read_sector(unsigned int lba, unsigned char *buffer)
{
    unsigned short *buf_words = (unsigned short *)buffer;
    int i;

    // Step 1: Wait for drive to not be busy
    ata_wait_bsy();

    // Step 2: Send sector address (LBA mode)
    // LBA is 28-bit: bits 0-7 go to LOW, 8-15 to MID, 16-23 to HIGH, 24-27 to DRIVE reg
    outb(ATA_SEC_LBA_LOW, lba & 0xFF);                            // Bits 0-7
    outb(ATA_SEC_LBA_MID, (lba >> 8) & 0xFF);                     // Bits 8-15
    outb(ATA_SEC_LBA_HIGH, (lba >> 16) & 0xFF);                   // Bits 16-23
    outb(ATA_SEC_DRIVE, ATA_DRIVE_MASTER | ((lba >> 24) & 0x0F)); // Bits 24-27 + drive select

    // Step 3: Tell drive we want to read 1 sector
    outb(ATA_SEC_SECTOR_CNT, 1);

    // Step 4: Send READ command
    outb(ATA_SEC_COMMAND, ATA_CMD_READ);

    // Step 5: Wait for drive to finish and have data ready
    ata_wait_bsy();
    if (inb(ATA_SEC_STATUS) & ATA_STATUS_ERR)
    {
        print_string("ATA read error\n", COLOR_RED);
        return -1;
    }
    ata_wait_drq();

    // Step 6: Read 256 words (512 bytes) from data port
    for (i = 0; i < 256; i++)
    {
        buf_words[i] = inw(ATA_SEC_DATA); // inw reads 2 bytes at once
    }

    return 0; // Success
}

// Write 512 bytes to sector at address 'lba'
int ata_write_sector(unsigned int lba, unsigned char *buffer)
{
    unsigned short *buf_words = (unsigned short *)buffer;
    int i;

    // Step 1: Wait for drive to not be busy
    ata_wait_bsy();

    // Step 2: Send sector address (same as read)
    outb(ATA_SEC_LBA_LOW, lba & 0xFF);
    outb(ATA_SEC_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_SEC_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_SEC_DRIVE, ATA_DRIVE_MASTER | ((lba >> 24) & 0x0F));

    // Step 3: Tell drive we want to write 1 sector
    outb(ATA_SEC_SECTOR_CNT, 1);

    // Step 4: Send WRITE command
    outb(ATA_SEC_COMMAND, ATA_CMD_WRITE);

    // Step 5: Wait until drive is ready for data
    ata_wait_bsy();
    ata_wait_drq();

    // Step 6: Write 256 words (512 bytes) to data port
    for (i = 0; i < 256; i++)
    {
        outw(ATA_SEC_DATA, buf_words[i]); // outw writes 2 bytes at once
    }

    // Step 7: Read status to acknowledge write completion
    inb(ATA_SEC_STATUS);

    // Step 8: Wait for write to complete
    ata_wait_bsy();
    if (inb(ATA_SEC_STATUS) & ATA_STATUS_ERR)
    {
        print_string("ATA write error\n", COLOR_RED);
        return -1;
    }

    return 0; // Success
}