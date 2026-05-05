#ifndef ATA_H
#define ATA_H

#include "io.h"

void ata_init(void);
int ata_read_sector(unsigned int lba, void *buffer);
int ata_write_sector(unsigned int lba, const void *buffer);
int get_disk_size(void);

#endif