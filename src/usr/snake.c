#include "vga.h"
#include "include/drivers/keyboard.h"

/**
 * helper func
 */
void snake_redraw(int snake_pos[2], int tail_pos[100][2])
{
    vga_clear(COLOR_BLACK);

    for (int counter = 100; counter > 0; counter--)
    {
        vga_write_char(tail_pos[counter][0], tail_pos[counter][1], '@', COLOR_LIGHT_GREEN);
    }

    vga_write_char(snake_pos[0], snake_pos[1], '#', COLOR_LIGHT_GREEN);
}

/**
 * assign the last tail pos recursively
 */
void snake_tail_update(int snake_pos[2], int tail_pos[100][2])
{
    for (int counter = 99; counter > 0; counter--)
    {
        tail_pos[counter][0] = tail_pos[counter - 1][0];
        tail_pos[counter][1] = tail_pos[counter - 1][1];
    }
    tail_pos[0][0] = snake_pos[0];
    tail_pos[0][1] = snake_pos[1];
}

void snake_main()
{
    int snake_pos[2] = {0, 0};
    int tail_pos[100][2] = {0};
    tail_pos[0][0] = snake_pos[0];
    vga_clear(COLOR_BLACK);
    vga_write_char(snake_pos[0], snake_pos[1], '#', COLOR_LIGHT_GREEN);
    while (1)
    {
        char key = keyboard_read(0);
        if (key == '\b')
        {
            vga_clear(COLOR_BLACK);
            break;
        }
        else if (key == 'w')
        {
            snake_pos[1] -= 1;
            snake_tail_update(snake_pos, tail_pos);
            snake_redraw(snake_pos, tail_pos);
        }
        else if (key == 's')
        {
            snake_pos[1] += 1;
            snake_tail_update(snake_pos, tail_pos);
            snake_redraw(snake_pos, tail_pos);
        }
        else if (key == 'a')
        {
            snake_pos[0] -= 1;
            snake_tail_update(snake_pos, tail_pos);
            snake_redraw(snake_pos, tail_pos);
        }
        else if (key == 'd')
        {
            snake_pos[0] += 1;
            snake_tail_update(snake_pos, tail_pos);
            snake_redraw(snake_pos, tail_pos);
        }
        // kill logic
        if ((snake_pos[0] < 0 || snake_pos[0] >= 80) || (snake_pos[1] < 0 || snake_pos[1] >= 25))
        {
            vga_clear(COLOR_BLACK);
            break;
        }
    }
}