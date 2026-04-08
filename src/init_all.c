#include "init_all.h"
#include "core/idt.h"
#include "include/drivers/irq.h"
#include "include/drivers/time/timer.h"
#include "include/drivers/keyboard.h"
#include "include/drivers/time/pit.h"
#include "include/drivers/cmos.h"
#include "core/gdt.h"
#include "vga.h"

void init_all()
{
    //__asm__ volatile("cli"); // DISABLE INTERUPTS
    gdt_init();
    idt_init();
    irq_remap();
    // pit_init();
    //__asm__ volatile("sti");
    time_init();
    // keyboard_init();
    cmos_init();
    print_string("Initialization complete!\n", COLOR_LIGHT_GREEN);
    __asm__ volatile("sti"); // INTERUPTS
    print_string("Starting kernel...\n", COLOR_LIGHT_GREEN);
}