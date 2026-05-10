# `keyboard.c`

## Dependencies

[include/drivers/keyboard.h](../../include/drivers/keyboard.h)<br>
[include/drivers/io.h](../../include/drivers/io.h)<br>
[vga.h](vga.h)<br>
[magics.h](magics.h)<br>
stdint.h(stander lib)

## Functions

### `Keyboard IRQ Handler`

Type: void<br>
handles Keyboard IRQ requests

### `ScanCode To ASCII`

Type: char<br>
converts scancodes obtained from the keyboard to the ASCII format

### `unsigned char keyboard_wait(void)`

Type: unsigned char<br>
waits for the keyboard to say that a key has been pressed then reads the value

**Returns:**

- `inb(KEYBOARD_DATA_PORT)` -> the key pressed

### `Keyboard init(keyboard_init)`

Note: INIT func<br>
Type: void<br>
Inits the Keyboard for use

### `Set Keyboard Repeat Delay(keyboard_set_repeat_delay)`

Type: void<br>
sets the delay before repeating to any of these valuse<br>

- 250ms
- 500ms
- 750ms
- 1s (1000ms)

**Parameters:**

- `unsigned int delay`
  the delay to set to

### `Set Keyboard Repeat Rate(keyboard_set_repeat_rate)`

Type: void<br>
sets the repeat rate for the keyboard to any of these values<br>

- 30cps
- 26.7cps
- 24cps
- 21.8cps
- 20.7cps
- 18.5cps
- 17.1cps
- 16cps
- 15cps
- 13.3cps
- 12cps
- 10.9cps
- 10cps
- 9.2cps
- 8.6cps
- 8cps
- 7.5cps
- 6.7cps
- 6cps
- 5.5cps
- 5cps
- 4.6cps
- 4.3cps
- 4cps
- 3.7cps
- 3.3cps
- 3cps
- 2.7cps
- 2.5cps
- 2.3cps
- 2.1cps
- 2cps

**Parameters:**

- `float rate_cps`
  MUST be one of the above values

### `Keyboard Read(keyboard_read)`

Type: char
Reads the keyboard for keypresses CAN handle shifts

**Parameters:**

- `int *shift_pressed`
  helps to convert from normal keys to shifted

**Returns:**

- `scancode_to_ascii(scancode, internal_shift)` -> converts the scancode read to a ASCII key to be returned
