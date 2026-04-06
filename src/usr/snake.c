#include "vga.h"
#include "include/drivers/keyboard.h"

void snake_main()
{
    vga_clear(COLOR_BLACK);
    int snake_pos[2] = {0, 0};
    while (1)
    {
        vga_clear(COLOR_BLACK);
        vga_write_char(snake_pos[0], snake_pos[1], '#', COLOR_GREEN);
        char key = keyboard_read(0);
        if (key == '\b')
        {
            break;
        }
        else if (key == 'w')
        {
            snake_pos[1] -= 1;
        }
        else if (key == 's')
        {
            snake_pos[1] += 1;
        }
        else if (key == 'a')
        {
            snake_pos[0] -= 1;
        }
        else if (key == 'd')
        {
            snake_pos[0] += 1;
        }
        // kill logic
        if ((snake_pos[0] < 0 || snake_pos[0] >= 80) || (snake_pos[1] < 0 || snake_pos[1] >= 25))
        {
            break;
        }
    }
}