# `vga.c`

## Dependencies

[vga.h](vga.h)<br/>
[include/drivers/io.h](include/drivers/io.h)<br/>
[lib/string.h](lib/string.h)<br/>
[magics.h](magics.h)<br/>

## Functions

### `void vga_write_char(int x, int y, char c, int color)`

**Parameters:**

- `int x`
- `int y`
- `char c`
- `int color`

**Returns:**

- `void` -> void return

---

### `void vga_write_string(int x, int y, const char *str, int color)`

**Parameters:**

- `int x`
- `int y`
- `const char *str`
- `int color`

**Returns:** nothing / void

---

### `void vga_clear(int bgcolor)`

**Parameters:**

- `int bgcolor`

**Returns:** nothing / void

---

### `void vga_move_cursor(void)`

**Parameters:** none

**Returns:**

- `void` ->> void return

---

### `void vga_disable_cursor()`

**Parameters:** none

**Returns:** nothing / void

---

### `void vga_enable_cursor(void)`

**Parameters:** none

**Returns:** nothing / void

---

### `void print_char(char c, int color)`

**Parameters:**

- `char c`
- `int color`

**Returns:**

- `void` -> void return

---

### `void vga_scroll()`

**Parameters:** none

**Returns:** nothing / void

---

### `void print_string(const char *str, int color)`

**Parameters:**

- `const char *str`
- `int color`

**Returns:** nothing / void

---

### `void write_bg_color(int x, int y, int color)`

**Parameters:**

- `int x`
- `int y`
- `int color`

**Returns:**

- `void` -> void return
