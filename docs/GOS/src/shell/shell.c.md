# `src/shell/shell.c`

## Dependencies

[vga.h](vga.h\)
[commands/default_cmds.h](../../commands/default_cmds.h\)
[lib/string.h](../../lib/string.h\)
[magics.h](magics.h\)

## Functions

### `int parse_args(const char *input, char argv[][64], int max_args)`

**Parameters:**
- `const char *input`
- `char argv[][64]`
- `int max_args`

**Returns:**
- `argc` → argc

---

### `void process_command(const char *input)`

**Parameters:**
- `const char *input`

**Returns:**
- `void` → void return

