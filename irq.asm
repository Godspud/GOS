[BITS 32]
extern irq_handler

global irq0_handler
global irq_dummy
global isr_dummy

irq0_handler:
    cli
    pusha
    call irq_handler
    popa
    sti
    iret

irq_dummy:
    pusha
    mov al, 0x20
    out 0xA0, al
    out 0x20, al
    popa
    iret


isr_dummy:
    cli
.hang:
    hlt
    jmp .hang