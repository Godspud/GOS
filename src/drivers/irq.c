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