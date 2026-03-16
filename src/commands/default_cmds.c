#include "default_cmds.h"
#include "vga.h"
#include "keyboard.h"
#include "string.h"
#include "io.h"

static void cmd_help(int argc, char **argv);
static void cmd_clear(int argc, char **argv);
static void cmd_echo(int argc, char **argv);
static void cmd_color(int argc, char **argv);
static void cmd_version(int argc, char **argv);
static void cmd_reboot(int argc, char **argv);
static void cmd_delay(int argc, char **argv);
static void cmd_rate(int argc, char **argv);

command_entry default_cmds[] = {
    {"help", cmd_help},
    {"clear", cmd_clear},
    {"echo", cmd_echo},
    {"color", cmd_color},
    {"version", cmd_version},
    {"reboot", cmd_reboot},
    {"delay", cmd_delay},
    {"rate", cmd_rate},
    {"", 0}};

static void cmd_help(int argc, char **argv)
{
    print_string("Available commands:\n", COLOR_WHITE);
    print_string("  help     - Show this help\n", COLOR_LIGHT_GREY);
    print_string("  clear    - Clear screen\n", COLOR_LIGHT_GREY);
    print_string("  echo     - Print text\n", COLOR_LIGHT_GREY);
    print_string("  color    - Set text color (0-15)\n", COLOR_LIGHT_GREY);
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
    for (int counter = 1; counter < argc; counter++)
    {
        const char *part = argv[counter];
        int counter_2 = 0;
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
        if (counter < argc - 1)
            print_string(" ", COLOR_WHITE);
    }
}

static void cmd_color(int argc, char **argv)
{
    print_string("Color command not implemented yet\n", COLOR_LIGHT_RED);
}

static void cmd_version(int argc, char **argv)
{
    print_string("OS v0.1.0\n", COLOR_LIGHT_CYAN);
}

static void cmd_reboot(int argc, char **argv)
{
    print_string("Rebooting...\n", COLOR_LIGHT_RED);
    outb(0x64, 0xFE);
    while (1)
        ;
}

static void cmd_delay(int argc, char **argv)
{
    keyboard_set_repeat_delay(argc);
}

static void cmd_rate(int argc, char **argv)
{
    keyboard_set_repeat_rate(argc);
}