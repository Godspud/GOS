# `src/usr/snake.c`

## Dependencies

[vga.h](vga.h\)
[include/drivers/keyboard.h](../../include/drivers/keyboard.h\)
[lib/random.h](../../lib/random.h\)
[include/macros.h](../../include/macros.h\)
[magics.h](magics.h\)

## Functions

### `void snake_redraw(int snake_pos[2], int tail_pos[100][2], int food_pos[2], int tail_length)`

**Parameters:**
- `int snake_pos[2]`
- `int tail_pos[100][2]`
- `int food_pos[2]`
- `int tail_length`

**Returns:** nothing / void

---

### `void snake_tail_update(int prev_head_pos[2], int tail_pos[100][2], int tail_length)`

**Parameters:**
- `int prev_head_pos[2]`
- `int tail_pos[100][2]`
- `int tail_length`

**Returns:** nothing / void

---

### `void food_gen(int food_pos[2])`

**Parameters:**
- `int food_pos[2]`

**Returns:** nothing / void

---

### `void check_food_pickup(int snake_pos[2], int food_pos[2], int *tail_length)`

**Parameters:**
- `int snake_pos[2]`
- `int food_pos[2]`
- `int *tail_length`

**Returns:** nothing / void

---

### `void move_snake(int direction, int snake_pos[2], int tail_pos[100][2], int food_pos[2], int *tail_length)`

**Parameters:**
- `int direction`
- `int snake_pos[2]`
- `int tail_pos[100][2]`
- `int food_pos[2]`
- `int *tail_length`

**Returns:** nothing / void

---

### `void snake_main()`

**Parameters:** none

**Returns:** nothing / void

