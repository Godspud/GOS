#include "vga.h"
#include "include/drivers/keyboard.h"

void snake_main()
{
    while (1)
    {
        vga_write_char(0, 0, " ", COLOR_GREEN);
    }
}