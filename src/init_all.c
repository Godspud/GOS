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

extern void irq0_handler(void);

void init_all()
{
    gdt_init();
    idt_init();
    irq_remap();
    pit_init();
    time_init();
    keyboard_init();
    cmos_init();
    ata_init();
    print_string("IRQ0 addr: ", COLOR_LIGHT_GREEN);
    print_string("Initialization complete!\n", COLOR_LIGHT_GREEN);
    __asm__ volatile("sti"); // INTERUPTS
    print_string("Starting kernel...\n", COLOR_LIGHT_GREEN);
}