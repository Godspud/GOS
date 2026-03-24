#include "include/drivers/io.h"

int frenqucy;
int PIT_BASE = 0;

void pit_init()
{
    // amount of times per second pit will be triggered
    frenqucy = 1000;
    int divisor = PIT_BASE / frenqucy;
    // 0x43 is the PIT add
    // 0x36 is in bin 00110110
    // first 2 bits(00) selects the counter
    // next 2 bits(11) tell PIT to expect a 16 bit value
    // next 3 bits(011) set timer to sq wave gen
    // last bit(0) use 16 bit bin format
    outb(0x43, 0x36);
    // 0xFF is 255
    // 0xFF is 0b11111111
    outb(0x40, divisor & 0xFF);
    // move divisor bu 8 bits
    // then mask by 0xFF
    outb(0x40, (divisor >> 8) & 0xFF);
}
