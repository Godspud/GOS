static unsigned long int next = 19752;

unsigned int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

unsigned int srand(unsigned int seed)
{
    next = seed;
    return rand();
}