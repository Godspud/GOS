#include "include/drivers/keyboard.h"
#include "vga.h"
#include "cmd_processor.h"
#include "commands/default_cmds.h"

static char input_buffer[256];
static int input_pos = 0;

void kernel_main()
{
    int shift_pressed = 0;
    char input_char;

    vga_clear(COLOR_BLACK);
    vga_enable_cursor();
    keyboard_init();
    print_string("================================================\n", COLOR_LIGHT_CYAN);
    print_string("            Welcome to OS!\n", COLOR_WHITE);
    print_string("================================================\n\n", COLOR_LIGHT_CYAN);

    print_string("Keyboard initialized!\n", COLOR_LIGHT_GREEN);
    print_string("Start typing below:\n\n", COLOR_LIGHT_GREY);
    print_string("> ", COLOR_LIGHT_GREEN);

    while (1)
    {
        vga_write_string(72, 0, "Shift: ", COLOR_LIGHT_MAGENTA);
        input_char = keyboard_read(&shift_pressed);
        if (input_char == '\n')
        {
            print_char('\n', COLOR_WHITE);
            input_buffer[input_pos] = '\0';
            process_command(input_buffer);
            input_pos = 0;
        }
        if (input_char != 0)
        {
            if (input_char == '\n')
            {
                print_char('\n', COLOR_WHITE);
                input_pos = 0;
                print_string("> ", COLOR_LIGHT_GREEN);
            }
            else if (input_char == '\b')
            {
                if (input_pos > 0)
                {
                    input_pos--;
                    print_char('\b', COLOR_WHITE);
                }
            }
            else if (input_char >= 32 && input_char <= 126)
            {
                if (input_pos < 255)
                {
                    input_buffer[input_pos++] = input_char;
                    print_char(input_char, COLOR_WHITE);
                }
            }
        }
    }
}