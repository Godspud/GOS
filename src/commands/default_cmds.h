#ifndef COMMANDS_H
#define COMMANDS_H

typedef void (*cmd_handler)(int argc, char **argv);
typedef struct
{
    char cmd[16];
    cmd_handler handler;
} command_entry;

extern command_entry default_cmds[];

void process_command(const char *input);

#endif