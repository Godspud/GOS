[BITS 32]
extern irq_handler

global irq0_handler

irq0_handler:
    cli
    pusha
    call irq_handler
    popa
    sti
    iret