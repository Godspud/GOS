#include "vga.h"
#include "include/drivers/keyboard.h"
#include "lib/random.h"

/**
 * helper func
 */
void snake_redraw(int snake_pos[2], int tail_pos[100][2], int food_pos[2], int tail_length)
{
    vga_clear(COLOR_BLACK);

    for (int counter = tail_length; counter > 0; counter--)
    {
        vga_write_char(tail_pos[counter][0], tail_pos[counter][1], '@', COLOR_LIGHT_GREEN);
    }

    vga_write_char(snake_pos[0], snake_pos[1], '#', COLOR_LIGHT_GREEN);

    vga_write_char(food_pos[0], food_pos[1], 'X', COLOR_RED);
}

/**
 * assign the last tail pos recursively
 */
void snake_tail_update(int snake_pos[2], int tail_pos[100][2], int tail_length)
{
    for (int counter = tail_length - 1; counter > 0; counter--)
    {
        tail_pos[counter][0] = tail_pos[counter - 1][0];
        tail_pos[counter][1] = tail_pos[counter - 1][1];
    }
    tail_pos[0][0] = snake_pos[0];
    tail_pos[0][1] = snake_pos[1];
}

void food_gen(int food_pos[2])
{
    food_pos[0] = rand() % 80;
    food_pos[1] = rand() % 25;
}

void check_food_pickup(int snake_pos[2], int food_pos[2], int tail_length)
{
    if (snake_pos[0] == food_pos[0] && snake_pos[1] == food_pos[1])
    {
        food_gen(food_pos);
        if (tail_length <= 100)
        {
            tail_length++;
        }
    }
}

void snake_main()
{
    int food_pos[2];
    food_gen(food_pos);
    int snake_pos[2] = {0, 0};
    int tail_pos[100][2] = {0};
    int tail_length = 0;
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
            snake_tail_update(snake_pos, tail_pos, tail_length);
            check_food_pickup(snake_pos, food_pos, tail_length);
            snake_redraw(snake_pos, tail_pos, food_pos, tail_length);
        }
        else if (key == 's')
        {
            snake_pos[1] += 1;
            snake_tail_update(snake_pos, tail_pos, tail_length);
            check_food_pickup(snake_pos, food_pos, tail_length);
            snake_redraw(snake_pos, tail_pos, food_pos, tail_length);
        }
        else if (key == 'a')
        {
            snake_pos[0] -= 1;
            snake_tail_update(snake_pos, tail_pos, tail_length);
            check_food_pickup(snake_pos, food_pos, tail_length);
            snake_redraw(snake_pos, tail_pos, food_pos, tail_length);
        }
        else if (key == 'd')
        {
            snake_pos[0] += 1;
            snake_tail_update(snake_pos, tail_pos, tail_length);
            check_food_pickup(snake_pos, food_pos, tail_length);
            snake_redraw(snake_pos, tail_pos, food_pos, tail_length);
        }
        // kill logic
        if ((snake_pos[0] < 0 || snake_pos[0] >= 80) || (snake_pos[1] < 0 || snake_pos[1] >= 25))
        {
            vga_clear(COLOR_BLACK);
            break;
        }
    }
}