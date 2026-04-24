#include "vga.h"
#include "shell/shell.h"
#include "shell/commands/default_cmds.h"
#include "init_all.h"
#include "include/drivers/keyboard.h"
#include "include/drivers/time/timer.h"
#include "include/drivers/cmos.h"
#include "core/idt.h"
#include "include/drivers/ata.h"
#include "include/drivers/fs/fs.h"
#include <string.h>

extern volatile unsigned int ticks;

static char input_buffer[256];
static int input_pos = 0;

void kernel_main()
{
    int shift_pressed = 0;
    char input_char;
    int keyboard_response = 0b00;
    char ticks_str[20];

    // vga_clear(COLOR_BLACK);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    ata_write_sector(1, &buffer); // Clear sector 1 for testing
    fs_create_file("test", "txt", "Hello, World!", 13);
    vga_enable_cursor();
    print_string("================================================\n", COLOR_LIGHT_CYAN);
    print_string("            Welcome to OS!\n", COLOR_WHITE);
    print_string("================================================\n\n", COLOR_LIGHT_CYAN);
    print_string("Start typing below:\n\n", COLOR_LIGHT_GREY);
    print_string("> ", COLOR_LIGHT_GREEN);
    idt_init();

    while (1)
    {
        cmos_get_time(&cmos_current_time);
        char time[9];
        time[0] = (cmos_current_time.hours / 10) + '0';
        time[1] = (cmos_current_time.hours % 10) + '0';
        time[2] = ':';
        time[3] = (cmos_current_time.minutes / 10) + '0';
        time[4] = (cmos_current_time.minutes % 10) + '0';
        time[5] = ':';
        time[6] = (cmos_current_time.seconds / 10) + '0';
        time[7] = (cmos_current_time.seconds % 10) + '0';
        time[8] = '\0';
        vga_write_string(66, 0, "Time: ", COLOR_LIGHT_MAGENTA);
        vga_write_string(72, 0, time, COLOR_LIGHT_MAGENTA);
        // Convert ticks to string
        unsigned int temp = ticks;
        int len = 0;
        if (temp == 0)
        {
            ticks_str[len++] = '0';
        }
        else
        {
            while (temp > 0)
            {
                ticks_str[len++] = (temp % 10) + '0';
                temp /= 10;
            }
        }
        ticks_str[len] = '\0';
        // Reverse the string
        for (int i = 0; i < len / 2; i++)
        {
            char t = ticks_str[i];
            ticks_str[i] = ticks_str[len - 1 - i];
            ticks_str[len - 1 - i] = t;
        }
        vga_write_string(0, 24, ticks_str, COLOR_LIGHT_MAGENTA);
        input_char = keyboard_read(&shift_pressed);
        if (input_char == '\n')
        {
            if (input_pos > 0)
            {
                print_char('\n', COLOR_WHITE);
                input_buffer[input_pos] = '\0';
                process_command(input_buffer);
                input_pos = 0;
            }
        }
        if (input_char != '\0')
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