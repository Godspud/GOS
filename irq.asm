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

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp        ; pass pointer to registers
    call irq_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds

    popa

    mov al, 0x20
    out 0x20, al

    sti
    iret

irq_dummy:
    pusha
    mov al, 0x20
    out 0x20, al
    iret


isr_dummy:
    hlt
    iret