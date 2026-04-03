#include "vga.h"
#include "include/drivers/io.h"
#include "string.h"

/** Functions:
- vga_write_char: Writes a single character at the specified (x, y) position with the given color.
- vga_write_string: Writes a null-terminated string starting at the specified (x,y) position with the given color.
- vga_clear: Clears the entire screen and fills it with the specified background color.
- vga_move_cursor: Updates the hardware cursor position to match the current cursor_x and cursor_y values.
- vga_disable_cursor: Disables the hardware cursor.
- vga_enable_cursor: Enables the hardware cursor.
- vga_scroll: Scrolls the screen up by one line, clearing the last line.
- print_char: Prints a single character at the current cursor position, handling special characters like newline, backspace, and tab.
- print_string: Prints a null-terminated string starting at the current cursor position, handling special characters as needed.
*/

int cursor_x = 0;
int cursor_y = 0;
static int cursor_enabled = 1;
/**
 * vga_write_char: Writes a single character at the specified (x, y) position with the given color.
 */
void vga_write_char(int x, int y, char c, int color)
{
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT)
        // if the x coord is outside of the screen on the left side or right side
        // or if the y coord is outside of the screen on the top or bottom
        return;
    // abs(short vga) = treat the VGA_MEMORY as a unsinged short
    unsigned short *vga = (unsigned short *)VGA_MEMORY;
    int offset = y * VGA_WIDTH + x;
    // move color to the left 8 bits and combine it with the character
    vga[offset] = (color << 8) | (unsigned char)c;
}
/**
 * vga_write_string: Writes a null-terminated string starting at the specified (x,y) position with the given color.
 */
void vga_write_string(int x, int y, const char *str, int color)
{
    int counter = 0;
    while (str[counter] != '\0')
    {
        vga_write_char(x + counter, y, str[counter], color);
        counter++;
    }
}
/**
 * vga_clear: Clears the entire screen and fills it with the specified background color.
 */
void vga_clear(int bgcolor)
{
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            vga_write_char(x, y, ' ', bgcolor);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_move_cursor();
}
/**
 * vga_move_cursor: Updates the hardware cursor position to match the current cursor_x and cursor_y values.
 */
void vga_move_cursor(void)
{
    // do ts later
    if (!cursor_enabled)
        return;
    int pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);

    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}
/**
 * vga_disable_cursor: Disables the hardware cursor.
 */
void vga_disable_cursor()
{
    // do ts later
    cursor_enabled = 0;
    outb(0x3D4, 10);
    outb(0x3D5, 0x20);
}
/**
 * vga_enable_cursor: Enables the hardware cursor.
 */
void vga_enable_cursor(void)
{
    // do ts later
    cursor_enabled = 1;
    outb(0x3D4, 10);
    outb(0x3D5, 14);
    outb(0x3D4, 11);
    outb(0x3D5, 15);
    vga_move_cursor();
}
/**
 * print_char: Prints a single character at the current cursor position, handling special characters like newline, backspace, and tab.
 */
void print_char(char c, int color)
{
    if (c == '\n')
    {
        // reset x coord of cursor and move it down one line
        cursor_x = 0;
        cursor_y++;
    }
    // \b is backspace
    else if (c == '\b')
    {
        if (cursor_x > 0)
        {
            cursor_x--;
            vga_write_char(cursor_x, cursor_y, ' ', color);
        }
        else if (cursor_y > 0)
        {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
            vga_write_char(cursor_x, cursor_y, ' ', color);
        }
        vga_move_cursor();
        return;
    }
    // i think \t is tab
    else if (c == '\t')
    {
        // move the cursor to +4 of the old one then invrese the bin format
        cursor_x = (cursor_x + 4) & ~3;
    }
    else if (c >= 32 && c <= 126)
    {
        // if the character is a printable character, print it and move the cursor to the right
        vga_write_char(cursor_x, cursor_y, c, color);
        cursor_x++;
    }
    if (cursor_x >= VGA_WIDTH)
    {
        // alt newline if the cursor goes past the right side of the screen
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT)
    {
        vga_scroll();
        cursor_y = VGA_HEIGHT - 1;
    }

    vga_move_cursor();
}
/**
 * vga_scroll: Scrolls the screen up by one line, clearing the last line.
 */
void vga_scroll()
{
    unsigned short *vga = (unsigned short *)VGA_MEMORY;
    for (int y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            int src = (y + 1) * VGA_WIDTH + x;
            int dst = y * VGA_WIDTH + x;
            vga[dst] = vga[src];
        }
    }
    int last_line = (VGA_HEIGHT - 1) * VGA_WIDTH;
    for (int x = 0; x < VGA_WIDTH; x++)
    {
        vga[last_line + x] = (COLOR_BLACK << 8) | ' ';
    }
}
/**
 * print_string: Prints a null-terminated string starting at the current cursor position, handling special characters as needed.
 */
void print_string(const char *str, int color)
{
    int i = 0;
    while (str[i] != '\0')
    {
        print_char(str[i], color);
        i++;
    }
}