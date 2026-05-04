#ifndef VGA_H
#define VGA_H

extern int cursor_x;
extern int cursor_y;

void vga_write_char(int x, int y, char c, int color);
void vga_write_string(int x, int y, const char *str, int color);
void vga_clear(int bgcolor);
void vga_move_cursor(void);
void vga_disable_cursor(void);
void vga_enable_cursor(void);
void vga_scroll();
void print_char(char c, int color);
void print_string(const char *str, int color);

#endif