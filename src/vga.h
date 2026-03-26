#ifndef VGA_H
#define VGA_H

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GREY 7
#define COLOR_DARK_GREY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_LIGHT_BROWN 14
#define COLOR_WHITE 15

extern int cursor_x;
extern int cursor_y;

void vga_write_char(int x, int y, char c, int color);
void vga_write_string(int x, int y, const char *str, int color);
void vga_clear(int bgcolor);
void vga_move_cursor(void);
void vga_disable_cursor(void);
void vga_enable_cursor(void);
void vga_scroll(void);
void print_char(char c, int color);
void print_string(const char *str, int color);

#endif