#include "include/drivers/time/timer.h"
#include "include/drivers/io.h"
#include "include/drivers/keyboard.h"

void irq_handler(int irq)
{
    if (irq == 32)
    {
        tick();
    }
}

static inline void io_wait()
{
    __asm__ volatile("outb %%al, $0x80" : : "a"(0));
}

void irq_remap()
{
    // Mask all IRQs
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);

    // Start PIC init (ICW1)
    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    // Remap offsets (ICW2)
    outb(0x21, 0x20);
    io_wait(); // Master PIC vector offset 0x20
    outb(0xA1, 0x28);
    io_wait(); // Slave PIC vector offset 0x28
    // Tell Master/Slave wiring (ICW3)
    outb(0x21, 0x04);
    io_wait(); // Master has a slave on IRQ2 (binary 0100)
    outb(0xA1, 0x02);
    io_wait(); // Slave identity (connected to master's IRQ2)
    // Set 8086/88 mode (ICW4)
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    // Restore saved masks if any (optional)
    // outb(0x21, saved1);
    // outb(0xA1, saved2);

    // Unmask IRQ0 (timer) only
    outb(0x21, inb(0x21) & ~0x01);
}