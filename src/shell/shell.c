/*
int parse_args(const char *input, char argv[][64], int max_args)
{
    int argc = 0;
    int i = 0;
    int j = 0;
    int in_arg = 0;
    while (input[i] != '\0' && argc < max_args)
    {
        if (input[i] == ' ' || input[i] == '\n' || input[i] == '\t')
        {
            if (in_arg)
            {
                argv[argc][j] = '\0';
                argc++;
                j = 0;
                in_arg = 0;
            }
        }
        else
        {
            if (!in_arg)
            {
                in_arg = 1;
            }
            if (j < 63)
            {
                argv[argc][j++] = input[i];
            }
        }
        i++;
    }
    if (in_arg)
    {
        argv[argc][j] = '\0';
        argc++;
    }
    return argc;
}
*/