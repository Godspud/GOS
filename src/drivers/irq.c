#include "include/drivers/time/timer.h"
#include "include/drivers/io.h"

void send_eoi()
{
    outb(0x20, 0x20);
}
void irq_handler()
{
    tick();
    send_eoi();
}

int irq_remap()
{
    char mask_1 = inb(0x21);
    char mask_2 = inb(0xA1);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // init
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // set vector offest
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // restore masks
    outb(0x21, mask_1);
    outb(0xA1, mask_2);

    outb(0x21, inb(0x21) & ~0x01);
}