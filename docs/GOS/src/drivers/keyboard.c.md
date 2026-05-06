# `src/drivers/keyboard.c`

## Dependencies

[include/drivers/keyboard.h](../../include/drivers/keyboard.h\)
[include/drivers/io.h](../../include/drivers/io.h\)
[vga.h](vga.h\)
[stdint.h](stdint.h\)
[magics.h](magics.h\)
[stdint.h](stdint.h\)

## Functions

### `void keyboard_callback()`

**Parameters:** none

**Returns:** nothing / void

---

### `unsigned char keyboard_wait(void)`

**Parameters:** none

**Returns:**
- `inb(KEYBOARD_DATA_PORT)` → inb(KEYBOARD_DATA_PORT)

---

### `void keyboard_init(void)`

**Parameters:** none

**Returns:**
- `keyboard_init()` → keyboard_init()
- `((ack << 1) | pass)` → ((ack << 1) | pass)

---

### `void keyboard_set_repeat(int enabled)`

**Parameters:**
- `int enabled`

**Returns:** nothing / void

---

### `void keyboard_set_repeat_delay(unsigned int delay)`

**Parameters:**
- `unsigned int delay`

**Returns:** nothing / void

---

### `void keyboard_set_repeat_rate(float rate_cps)`

**Parameters:**
- `float rate_cps`

**Returns:** nothing / void

