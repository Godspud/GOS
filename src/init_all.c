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
#include "magics.h"

extern void irq0_handler(void);

void init_all()
{
    vga_clear(COLOR_BLACK);
    gdt_init();
    print_string("[OK] GDT\n", COLOR_LIGHT_GREEN);
    idt_init();
    print_string("[OK] IDT\n", COLOR_LIGHT_GREEN);
    irq_remap();
    print_string("[OK] PIC\n", COLOR_LIGHT_GREEN);
    pit_init();
    print_string("[OK] PIT\n", COLOR_LIGHT_GREEN);
    time_init();
    print_string("[OK] Timer\n", COLOR_LIGHT_GREEN);
    keyboard_init();
    print_string("[OK] Keyboard\n", COLOR_LIGHT_GREEN);
    cmos_init();
    print_string("[OK] CMOS\n", COLOR_LIGHT_GREEN);
    ata_init();
    print_string("[OK] ATA\n", COLOR_LIGHT_GREEN);
    fs_init();
    print_string("[OK] GOSFS\n", COLOR_LIGHT_GREEN);
    print_string("Initialization complete!\n", COLOR_LIGHT_GREEN);
    __asm__ volatile("sti"); // INTERUPTS
    print_string("Starting kernel...\n", COLOR_LIGHT_GREEN);
}