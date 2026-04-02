__attribute__((noreturn)) void exception_handler(void);
void exception_handler()
{
    __asm__ volatile("cli; hlt;");
    while (1)
        ; // Ensure the function never returns
}
