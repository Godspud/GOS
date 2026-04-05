#include "init_all.h"
#include "core/idt.h"
#include "include/drivers/irq.h"
#include "include/drivers/time/timer.h"
#include "include/drivers/keyboard.h"
#include "include/drivers/time/pit.h"
#include "include/drivers/cmos.h"

void init_all()
{
    keyboard_init();
    irq_remap();
    idt_init();
    pit_init();
    time_init();
    cmos_init();
    //__asm__ volatile("sti"); // INTERUPTS
}