#include "vga.h"
#include "include/drivers/keyboard.h"

void snake_main()
{
    vga_clear(COLOR_BLACK);
    while (1)
    {
        vga_write_char(0, 0, '#', COLOR_GREEN);
        char key = keyboard_read(0);
        if (key == '\b')
        {break;}
    }
}