[BITS 32]

extern kernel_main

magic    equ 0x1BADB002
flags    equ 0x0
checksum equ -(magic + flags)

section .multiboot
    dd magic
    dd flags
    dd checksum

section .bss
    align 16
    stack_bottom:
        resb 16384
    stack_top:
    

section .text
    global start

start:
    cli

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, stack_top
    call kernel_main