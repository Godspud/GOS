# `default_cmds.h`

## Structs

### Command Entry(`command_entry`)

**Fields:**

- `char cmd[16]` The command string to match against user input
- `cmd_handler handler` Function pointer to the command handler

## Externs

- `extern command_entry default_cmds[]`

## Functions

Function declaration for

1. [process_command](../shell.c#void-process_commandconst-char-input)
