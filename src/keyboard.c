#include "keyboard.h"
#include "io.h"

/*
- get_time_ms: Returns the current time in milliseconds based on the timer ticks.
- scancode_to_ascii: Converts a keyboard scancode to its corresponding ASCII character, taking into account whether the Shift key is pressed.
- keyboard_wait: Waits for a key press and returns the scancode of the pressed key.
- keyboard_init: Initializes the keyboard controller and sets up necessary state for handling keyboard input.
- keyboard_set_repeat: Enables or disables key repeat functionality.
- keyboard_set_repeat_delay: Sets the delay before key repeat starts when a key is held down.
- keyboard_set_repeat_rate: Sets the rate at which keys repeat when held down.
- keyboard_read: Reads a key press from the keyboard, returning the corresponding ASCII character and updating the shift_pressed state.
*/

unsigned int keyboard_repeat_delay = 1000000;
unsigned int keyboard_repeat_rate = 5000;

static int repeat_enabled = 1;
static int last_scancode = 0;
static int last_key_pressed = 0;
static unsigned int key_press_time = 0;

static unsigned int timer_ticks = 0;
/**
 * get_time_ms: Returns the current time in milliseconds based on the timer ticks.
 */
static unsigned int get_time_ms(void)
{
    return timer_ticks;
}
/**
 * scancode_to_ascii: Converts a keyboard scancode to its corresponding ASCII character, taking into account whether the Shift key is pressed.
 */
char scancode_to_ascii(unsigned char scancode, int shift_pressed)
{
    static const char normal[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
        0, ' '};

    static const char shifted[] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
        0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
        0, ' '};
    // if the scancode is greater than or equal to 59, it's not a valid key we want to handle, so return 0
    if (scancode >= 59)
        return 0;
    // if shift is pressed, return the shifted character, otherwise return the normal character
    if (shift_pressed)
    {
        return shifted[scancode];
    }
    else
    {
        return normal[scancode];
    }
}
/**
 * keyboard_wait: Waits for a key press and returns the scancode of the pressed key.
 */
unsigned char keyboard_wait(void)
{
    // wait until the keyboard status port indicates that a key has been pressed (bit 0 is set)
    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
        ;
    return inb(KEYBOARD_DATA_PORT);
}
/**
 * keyboard_init: Initializes the keyboard controller and sets up necessary state for handling keyboard input.
 */
void keyboard_init(void)
{
    // wait until the keyboard controller is ready to receive commands (bit 1 of the status port is clear)
    while ((inb(KEYBOARD_STATUS_PORT) & 0x02) != 0)
        ;

    outb(0x64, 0xAE);

    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) != 0)
    {
        inb(KEYBOARD_DATA_PORT);
    }

    last_scancode = 0;
    last_key_pressed = 0;
    key_press_time = 0;
    timer_ticks = 0;
}
/**
 * keyboard_set_repeat: Enables or disables key repeat functionality.
 */
void keyboard_set_repeat(int enabled)
{
    repeat_enabled = enabled;
}
/**
 * keyboard_set_repeat_delay: Sets the delay before key repeat starts when a key is held down.
 */
void keyboard_set_repeat_delay(unsigned int delay_ms)
{
    keyboard_repeat_delay = delay_ms;
}
/**
 * keyboard_set_repeat_rate: Sets the rate at which keys repeat when held down.
 */
void keyboard_set_repeat_rate(unsigned int rate_ms)
{
    keyboard_repeat_rate = rate_ms;
}
/**
 * keyboard_read: Reads a key press from the keyboard, returning the corresponding ASCII character and updating the shift_pressed state.
 */
char keyboard_read(int *shift_pressed)
{
    // handle key repeat if enabled and a key is currently pressed
    static int internal_shift = 0;

    timer_ticks++;

    if ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
    {
        if (repeat_enabled && last_key_pressed && last_scancode != 0)
        {
            unsigned int current_time = get_time_ms();
            unsigned int hold_time = current_time - key_press_time;

            if (hold_time >= keyboard_repeat_delay)
            {
                unsigned int repeat_time = hold_time - keyboard_repeat_delay;

                if (repeat_time >= keyboard_repeat_rate)
                {
                    key_press_time = current_time;
                    char repeated_char = scancode_to_ascii(last_scancode, internal_shift);
                    if (repeated_char != 0)
                    {
                        return repeated_char;
                    }
                }
            }
        }
        return 0;
    }
    // read the scancode of the pressed key
    unsigned char scancode = keyboard_wait();
    // if the scancode has the high bit set, it means the key was released
    if (scancode & 0x80)
    {
        int released_code = scancode & 0x7F;

        if (released_code == 0x2A || released_code == 0x36)
        {
            internal_shift = 0;
            *shift_pressed = 0;
        }

        if (released_code == last_scancode)
        {
            last_key_pressed = 0;
        }

        return 0;
    }
    // if the scancode corresponds to a Shift key being pressed, update the internal shift state and the output parameter
    if (scancode == 0x2A || scancode == 0x36)
    {
        internal_shift = 1;
        *shift_pressed = 1;
        return 0;
    }
    // for any other key press, update the last scancode and key press time, and return the corresponding ASCII character
    last_scancode = scancode;
    last_key_pressed = 1;
    key_press_time = get_time_ms();

    return scancode_to_ascii(scancode, internal_shift);
}