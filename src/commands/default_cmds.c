#include "default_cmds.h"
#include "vga.h"
#include "keyboard.h"
#include "string.h"
#include "io.h"
#include "cmos.h"

/*
- cmd_help: Displays a list of available commands and their descriptions.
- cmd_clear: Clears the screen.
- cmd_echo: Prints the provided arguments back to the screen, handling escaped quotes.
- cmd_version: Displays the current version of the OS.
- cmd_reboot: Reboots the system by sending a command to the keyboard controller.
- cmd_delay: Sets the keyboard repeat delay based on the provided argument.
- cmd_rate: Sets the keyboard repeat rate based on the provided argument.
*/

static void cmd_help(int argc, char **argv);
static void cmd_clear(int argc, char **argv);
static void cmd_echo(int argc, char **argv);
static void cmd_color(int argc, char **argv);
static void cmd_version(int argc, char **argv);
static void cmd_reboot(int argc, char **argv);
static void cmd_delay(int argc, char **argv);
static void cmd_rate(int argc, char **argv);
static void cmd_time(int argc, char **argv);

// Command table mapping command strings to their handler functions
// The last entry has an empty command string to indicate the end of the table
command_entry default_cmds[] = {
    {"help", cmd_help},
    {"clear", cmd_clear},
    {"echo", cmd_echo},
    {"version", cmd_version},
    {"reboot", cmd_reboot},
    {"delay", cmd_delay},
    {"rate", cmd_rate},
    {"time", cmd_time},
    {"", 0}};

static void cmd_help(int argc, char **argv)
{
    print_string("Available commands:\n", COLOR_WHITE);
    print_string("  help     - Show this help\n", COLOR_LIGHT_GREY);
    print_string("  clear    - Clear screen\n", COLOR_LIGHT_GREY);
    print_string("  echo     - Print text\n", COLOR_LIGHT_GREY);
    print_string("  version  - Show version\n", COLOR_LIGHT_GREY);
    print_string("  reboot   - Restart system\n", COLOR_LIGHT_GREY);
    print_string("  delay    - Set key repeat delay\n", COLOR_LIGHT_GREY);
    print_string("  rate     - Set key repeat rate\n", COLOR_LIGHT_GREY);
}

static void cmd_clear(int argc, char **argv)
{
    vga_clear(COLOR_BLACK);
}

static void cmd_echo(int argc, char **argv)
{
    // Start from argv[1] to skip the command itself
    for (int counter = 1; counter < argc; counter++)
    {
        const char *part = argv[counter];
        int counter_2 = 0;
        // do till the char is \0 end of sstr
        while (part[counter_2] != '\0')
        {
            if (part[counter_2] == '\\' && part[counter_2 + 1] == '"')
            {
                print_char('"', COLOR_WHITE);
                counter_2 += 2;
            }
            else if (part[counter_2] == '\\' && part[counter_2 + 1] == '\'')
            {
                print_char('\'', COLOR_WHITE);
                counter_2 += 2;
            }
            else if (part[counter_2] == '"')
            {
                counter_2++;
            }
            else if (part[counter_2] == '\'')
            {
                counter_2++;
            }
            else
            {
                print_char(part[counter_2], COLOR_WHITE);
                counter_2++;
            }
        }
        // Print a space between arguments, but not after the last one
        if (counter < argc - 1)
            print_string(" ", COLOR_WHITE);
    }
}

static void cmd_version(int argc, char **argv)
{
    print_string("OS v0.2.0\n", COLOR_LIGHT_CYAN);
}

static void cmd_reboot(int argc, char **argv)
{
    print_string("Rebooting...\n", COLOR_LIGHT_RED);
    // Send the reboot command to the keyboard controller
    outb(0x64, 0xFE);
    while (1)
        ;
}

static void cmd_time(int argc, char **argv)
{
    cmos_get_time(&cmos_current_time);
    print_string("Current time: ", COLOR_GREEN);
    print_char((cmos_current_time.hours / 10) + '0', COLOR_GREEN);
    print_char((cmos_current_time.hours % 10) + '0', COLOR_GREEN);
    print_char(':', COLOR_RED);
    print_char((cmos_current_time.minutes / 10) + '0', COLOR_GREEN);
    print_char((cmos_current_time.minutes % 10) + '0', COLOR_GREEN);
    print_char(':', COLOR_BLUE);
    print_char((cmos_current_time.seconds / 10) + '0', COLOR_GREEN);
    print_char((cmos_current_time.seconds % 10) + '0', COLOR_GREEN);
    print_char('\n', COLOR_GREEN);
}

static void cmd_delay(int argc, char **argv)
{
    keyboard_set_repeat_delay(argc);
}

static void cmd_rate(int argc, char **argv)
{
    keyboard_set_repeat_rate(argc);
}