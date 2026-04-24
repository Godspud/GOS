#include "include/drivers/ata.h"
#include "vga.h"
static unsigned int min_u32(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}

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
    return;
}

// Initialize ATA driver
void ata_init(void)
{
    // Just wait for drive to be ready
    ata_wait_bsy();
}

// Read 512 bytes from sector at address 'lba'
int ata_read_sector(unsigned int lba, void *buffer)
{
    unsigned short sector_words[256];
    unsigned int bytes_to_copy;
    unsigned int buffer_size = 512 - sizeof(buffer);
    unsigned char *dst = (unsigned char *)buffer;
    unsigned char *src = (unsigned char *)sector_words;
    int i;

    // Step 1: Wait for drive to not be busy
    ata_wait_bsy();

    // Step 2: Send sector address (LBA mode)
    // LBA is 28-bit: bits 0-7 go to LOW, 8-15 to MID, 16-23 to HIGH, 24-27 to DRIVE reg
    outb(ATA_SEC_LBA_LOW, lba & 0xFF);                                            // Bits 0-7
    outb(ATA_SEC_LBA_MID, (lba >> 8) & 0xFF);                                     // Bits 8-15
    outb(ATA_SEC_LBA_HIGH, (lba >> 16) & 0xFF);                                   // Bits 16-23
    outb(ATA_SEC_DRIVE, ATA_TARGET_DRIVE | ATA_DRIVE_LBA | ((lba >> 24) & 0x0F)); // Bits 24-27 + drive select

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
        sector_words[i] = inw(ATA_SEC_DATA); // inw reads 2 bytes at once
    }
    bytes_to_copy = min_u32(buffer_size, 512);
    for (i = 0; i < (int)bytes_to_copy; i++)
        dst[i] = src[i];

    return 0; // Success
}

// Write 512 bytes to sector at address 'lba'
int ata_write_sector(unsigned int lba, const void *buffer)
{
    print_char('0', COLOR_RED); // Debug: entered write function
    unsigned char sector[512];
    const unsigned char *src = (const unsigned char *)buffer;
    const unsigned short *buf_words = (const unsigned short *)sector;
    int counter;

    // Copy all 512 bytes from buffer to sector
    for (counter = 0; counter < 512; counter++)
        sector[counter] = src[counter];
    ata_wait_bsy();
    print_char('1', COLOR_RED); // Debug: reached write function

    // Set up sector count and LBA first
    outb(ATA_SEC_SECTOR_CNT, 1);
    outb(ATA_SEC_LBA_LOW, lba & 0xFF);
    outb(ATA_SEC_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_SEC_LBA_HIGH, (lba >> 16) & 0xFF);
    print_char('2', COLOR_RED); // Debug: LBA set

    // Set drive/head register
    outb(ATA_SEC_DRIVE, ATA_TARGET_DRIVE | ATA_DRIVE_LBA | ((lba >> 24) & 0x0F));
    print_char('3', COLOR_RED); // Debug: drive selected

    // Small delay for drive select to take effect
    inb(ATA_SEC_STATUS);
    inb(ATA_SEC_STATUS);
    inb(ATA_SEC_STATUS);
    inb(ATA_SEC_STATUS);
    print_char('4', COLOR_RED); // Debug: delay done

    // Send WRITE command
    outb(ATA_SEC_COMMAND, ATA_CMD_WRITE);
    print_char('5', COLOR_RED); // Debug: command sent

    // Wait for DRQ (data request)
    ata_wait_drq();
    print_char('6', COLOR_RED); // Debug: drive ready for data

    // Write 256 words (512 bytes)
    for (counter = 0; counter < 256; counter++)
        outw(ATA_SEC_DATA, buf_words[counter]);
    print_char('7', COLOR_RED); // Debug: data written

    // Status read to acknowledge
    inb(ATA_SEC_STATUS);
    print_char('8', COLOR_RED); // Debug: status read

    // Wait for write to complete
    ata_wait_bsy();
    if (inb(ATA_SEC_STATUS) & ATA_STATUS_ERR)
    {
        print_string("ATA write error\n", COLOR_RED);
        return -1;
    }
    print_char('9', COLOR_RED); // Debug: write complete

    return 0;
}