[BITS 32]
extern irq_handler

global irq0_handler

irq0_handler:
    pusha
    call irq_handler
    popa
    iret