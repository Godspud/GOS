#include "include/drivers/time/timer.h"
#include "include/drivers/io.h"
#include "stdint.h"

void send_eoi()
{
    outb(0x20, 0x20);
}
void irq_handler()
{
    tick();
    send_eoi();
}

void irq_remap()
{
    uint8_t a1 = inb(0x21);
    uint8_t a2 = inb(0xA1);

    // start init
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // set offsets
    outb(0x21, 0x20); // master → 0x20
    outb(0xA1, 0x28); // slave  → 0x28

    // tell master/slave wiring
    outb(0x21, 0x04); // slave on IRQ2
    outb(0xA1, 0x02);

    // 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // restore masks
    outb(0x21, a1);
    outb(0xA1, a2);

    // UNMASK IRQ0 (timer)
    outb(0x21, inb(0x21) & ~0x01);
}