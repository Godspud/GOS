#ifndef KEYBOARD_H
#define KEYBOARD_H

extern unsigned int keyboard_repeat_rate;

void keyboard_set_repeat(int enabled);
void keyboard_set_repeat_delay(unsigned int delay_ms);
void keyboard_set_repeat_rate(unsigned int rate_ms);

char keyboard_read(int *shift_pressed);

unsigned char keyboard_wait(void);

void keyboard_init(void);

#endif