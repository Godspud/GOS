#include "include/drivers/keyboard.h"
#include "include/drivers/io.h"
#include "vga.h"
#include <stdint.h>
#include "magics.h"

#include <stdint.h>

void keyboard_callback()
{
    uint8_t scancode = inb(0x60); // THIS LINE IS REQUIRED

    // optional debug
    print_string("K", COLOR_LIGHT_RED);
}
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
unsigned int keyboard_repeat_rate = 1000000000;

static int repeat_enabled = 1;
static int last_scancode = 0;

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
    int ack = 0;
    int pass = 0;
    int counter = 0;
    int response = 0;
    // wait until the keyboard controller is ready to receive commands (bit 1 of the status port is clear)
    while ((inb(KEYBOARD_STATUS_PORT) & 0x02) != 0)
        ;
    outb(KEYBOARD_STATUS_PORT, 0xAE);
    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) != 0)
    {
        inb(KEYBOARD_DATA_PORT);
    }
    //
    // outb(0x60, 0xFF);
    // for (counter; counter < 3; counter++)
    //    ;
    //{
    //    while (!(inb(KEYBOARD_STATUS_PORT) & 0x01))
    //        ;
    //    response = inb(KEYBOARD_DATA_PORT);
    //    if (response == 0xFA)
    //    {
    //        ack = 1;
    //    }
    //    if (response == 0xAA)
    //    {
    //        pass = 1;
    //        print_string("PASS", COLOR_GREEN);
    //    }
    //    if (response == 0xFE)
    //    {
    //        print_string("RETURN", COLOR_RED);
    //        return keyboard_init();
    //    }
    //}

    last_scancode = 0;
    // return ((ack << 1) | pass);
}
/**
 * keyboard_set_repeat: Enables or disables key repeat functionality.
 */
void keyboard_set_repeat(int enabled)
{
    // TODO: UPGRADE TO THE SAME SYSTEM AS keyboard_set_repeat_delay
    repeat_enabled = enabled;
}
/**
 * keyboard_set_repeat_delay: Sets the delay before key repeat starts when a key is held down.
 * 0.25 sec or 0.5 sec or 0.75 sec or 1 sec
 *
 */
void keyboard_set_repeat_delay(unsigned int delay)
{
    enum
    {
        DELAY_250_MS = 0b00,
        DELAY_500_MS = 0b01,
        DELAY_750_MS = 0b10,
        DELAY_1000_MS = 0b11
    };
    switch (delay)
    {
    case 250:
        delay = DELAY_250_MS;
        break;
    case 500:
        delay = DELAY_500_MS;
        break;
    case 750:
        delay = DELAY_750_MS;
        break;
    case 1000:
        delay = DELAY_1000_MS;
        break;
    default:
        delay = DELAY_1000_MS;
        break;
    }
    outb(0x60, 0xF3);
    outb(0x60, delay);
}
/**
 * keyboard_set_repeat_rate: Sets the rate at which keys repeat when held down.
 */
void keyboard_set_repeat_rate(float rate_cps)
{
    unsigned int rate = rate_cps * 10;
    enum
    {
        RATE_30_CPS = 0x00,
        RATE_26_7_CPS = 0x01,
        RATE_24_CPS = 0x02,
        RATE_21_8_CPS = 0x03,
        RATE_20_7_CPS = 0x04,
        RATE_18_5_CPS = 0x05,
        RATE_17_1_CPS = 0x06,
        RATE_16_CPS = 0x07,
        RATE_15_CPS = 0x08,
        RATE_13_3_CPS = 0x09,
        RATE_12_CPS = 0x0A,
        RATE_10_9_CPS = 0x0B,
        RATE_10_CPS = 0x0C,
        RATE_9_2_CPS = 0x0D,
        RATE_8_6_CPS = 0x0E,
        RATE_8_CPS = 0x0F,
        RATE_7_5_CPS = 0x10,
        RATE_6_7_CPS = 0x11,
        RATE_6_CPS = 0x12,
        RATE_5_5_CPS = 0x13,
        RATE_5_CPS = 0x14,
        RATE_4_6_CPS = 0x15,
        RATE_4_3_CPS = 0x16,
        RATE_4_CPS = 0x17,
        RATE_3_7_CPS = 0x18,
        RATE_3_3_CPS = 0x19,
        RATE_3_CPS = 0x1A,
        RATE_2_7_CPS = 0x1B,
        RATE_2_5_CPS = 0x1C,
        RATE_2_3_CPS = 0x1D,
        RATE_2_1_CPS = 0x1E,
        RATE_2_CPS = 0x1F,
    };
    switch (rate)
    {
    case 300:
        rate = RATE_30_CPS;
        break;
    case 267:
        rate = RATE_26_7_CPS;
        break;
    case 24:
        rate = RATE_24_CPS;
        break;
    case 218:
        rate = RATE_21_8_CPS;
        break;
    case 207:
        rate = RATE_20_7_CPS;
        break;
    case 185:
        rate = RATE_18_5_CPS;
        break;
    case 171:
        rate = RATE_17_1_CPS;
        break;
    case 160:
        rate = RATE_16_CPS;
        break;
    case 150:
        rate = RATE_15_CPS;
        break;
    case 133:
        rate = RATE_13_3_CPS;
        break;
    case 120:
        rate = RATE_12_CPS;
        break;
    case 109:
        rate = RATE_10_9_CPS;
        break;
    case 100:
        rate = RATE_10_CPS;
        break;
    case 92:
        rate = RATE_9_2_CPS;
        break;
    case 86:
        rate = RATE_8_6_CPS;
        break;
    case 80:
        rate = RATE_8_CPS;
        break;
    case 75:
        rate = RATE_7_5_CPS;
        break;
    case 67:
        rate = RATE_6_7_CPS;
        break;
    case 55:
        rate = RATE_5_5_CPS;
        break;
    case 50:
        rate = RATE_5_CPS;
        break;
    case 46:
        rate = RATE_4_6_CPS;
        break;
    case 43:
        rate = RATE_4_3_CPS;
        break;
    case 40:
        rate = RATE_4_CPS;
        break;
    case 37:
        rate = RATE_3_7_CPS;
        break;
    case 33:
        rate = RATE_3_3_CPS;
        break;
    case 30:
        rate = RATE_3_CPS;
        break;
    case 27:
        rate = RATE_2_7_CPS;
        break;
    case 25:
        rate = RATE_2_5_CPS;
        break;
    case 23:
        rate = RATE_2_3_CPS;
        break;
    case 21:
        rate = RATE_2_1_CPS;
        break;
    case 20:
        rate = RATE_2_CPS;
        break;
    default:
        rate = RATE_2_CPS;
        break;
    }
    outb(0x60, 0xF3);
    outb(0x60, rate);
}
/**
 * keyboard_read: Reads a key press from the keyboard, returning the corresponding ASCII character and updating the shift_pressed state.
 */
char keyboard_read(int *shift_pressed)
{
    // handle key repeat if enabled and a key is currently pressed
    static int internal_shift = 0;

    if ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
    {
        return 0;
    }

    unsigned char scancode = inb(KEYBOARD_DATA_PORT);

    // read the scancode of the pressed key
    // unsigned char scancode = keyboard_wait();
    // if the scancode has the high bit set, it means the key was released
    if (scancode & 0x80)
    {
        int released_code = scancode & 0x7F;

        if (released_code == 0x2A || released_code == 0x36)
        {
            internal_shift = 0;
            *shift_pressed = 0;
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

    return scancode_to_ascii(scancode, internal_shift);
}