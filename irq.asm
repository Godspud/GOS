[BITS 32]
extern irq_handler

global irq0_handler
global irq_dummy
global isr_dummy

irq0_entry:
    pushad
    call irq0_handler
    popad
    mov al,0x20
    out 0x20, al
    iret

irq0_handler:
    cli
    pusha

    mov al, 0x20
    out 0x20, al

    call irq_handler

    popa
    iret

irq_dummy:
    pusha
    mov al, 0x20
    out 0x20, al
    iret


isr_dummy:
    hlt
    iret