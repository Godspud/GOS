#include "vga.h"
#include "include/drivers/keyboard.h"

void snake_main()
{
    vga_clear(COLOR_BLACK);
    while (1)
    {
        write_bg_color(0, 0, COLOR_GREEN);
    }
}