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
    pushad
    call irq_handler
    popad
    mov al, 0x20
    out 0x20, al
    iret
    ;cli
    ;pusha
    ;call irq_handler
    ;popa
    ;sti
    ;iret

irq_dummy:
    pusha
    mov al, 0x20
    out 0x20, al
    iret


isr_dummy:
    hlt
    iret