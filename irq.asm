[BITS 32]

global irq0_handler
global irq1_handler
global irq_common
global irq_dummy
global isr_dummy

extern irq_handler   ; C function

; =========================
; IRQ0 (PIT)
; =========================
irq0_handler:
    push 0
    push 32
    jmp irq_common

; =========================
; IRQ1 (keyboard)
; =========================
irq1_handler:
    push 0
    push 33
    jmp irq_common

; =========================
; GENERIC IRQ HANDLER
; =========================
irq_common:
    pusha

    ; set segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    add esp, 4

    popa
    add esp, 8

    ; send EOI (master PIC only for now)
    mov al, 0x20
    out 0x20, al

    iret

; =========================
; DUMMY IRQ HANDLER (for unused IRQs)
; =========================
irq_dummy:
    pusha

    mov al, 0x20
    out 0x20, al

    popa
    iret

; =========================
; DUMMY ISR (CPU exceptions)
; =========================
isr_dummy:
    cli
.hang:
    hlt
    jmp .hang