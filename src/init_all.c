#include "init_all.h"
#include "core/idt.h"
#include "include/drivers/irq.h"
#include "include/drivers/time/timer.h"
#include "include/drivers/keyboard.h"
#include "include/drivers/time/pit.h"
#include "include/drivers/cmos.h"
#include "core/gdt.h"
#include "vga.h"
#include "include/drivers/ata.h"
#include "include/drivers/fs/fs.h"

extern void irq0_handler(void);

void init_all()
{
    gdt_init();
    print_string("GDT initialized\n", COLOR_LIGHT_GREEN);
    idt_init();
    print_string("IDT initialized\n", COLOR_LIGHT_GREEN);
    irq_remap();
    print_string("PIC remapped\n", COLOR_LIGHT_GREEN);
    pit_init();
    print_string("PIT initialized\n", COLOR_LIGHT_GREEN);
    time_init();
    print_string("Timer initialized\n", COLOR_LIGHT_GREEN);
    keyboard_init();
    print_string("Keyboard initialized\n", COLOR_LIGHT_GREEN);
    cmos_init();
    print_string("CMOS initialized\n", COLOR_LIGHT_GREEN);
    // fs_init();
    print_string("Filesystem initialized\n", COLOR_LIGHT_GREEN);
    ata_init();
    print_string("ATA initialized\n", COLOR_LIGHT_GREEN);
    print_string("Initialization complete!\n", COLOR_LIGHT_GREEN);
    __asm__ volatile("sti"); // INTERUPTS
    print_string("Starting kernel...\n", COLOR_LIGHT_GREEN);
}