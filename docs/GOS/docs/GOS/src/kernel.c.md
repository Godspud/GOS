# `kernel.c`

## Dependencies

[vga.h](vga.h)<br/>
[shell/shell.h](shell/shell.h)<br/>
[shell/commands/default_cmds.h](shell/commands/default_cmds.h)<br/>
[init_all.h](init_all.h)<br/>
[include/drivers/keyboard.h](include/drivers/keyboard.h)<br/>
[include/drivers/time/timer.h](include/drivers/time/timer.h)<br/>
[include/drivers/cmos.h](include/drivers/cmos.h)<br/>
[core/idt.h](core/idt.h)<br/>
[include/drivers/ata.h](include/drivers/ata.h)<br/>
[include/drivers/fs/fs.h](include/drivers/fs/fs.h)<br/>
[lib/string.h](lib/string.h)<br/>
[magics.h](magics.h)<br/>

## Externs

- `extern volatile unsigned int ticks`

## Static Variables

- `static char input_buffer[256]`
- `static int input_pos`

## Functions

### `Kernel Main(kernel_main)`

Type: void<br/>
Main kernel loop, handles rendering the clock, reading keyboard input and dispatching commands
