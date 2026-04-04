#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct
{
    uint16_t isr_low;   // Lower 16 bits of handler function address
    uint16_t kernel_cs; // Kernel code segment selector
    uint8_t reserved;   // Reserved, set to 0
    uint8_t attributes; // Type and attributes
    uint16_t isr_high;  // Higher 16 bits of handler function address
} __attribute__((packed)) idt_entry_t;

typedef struct
{
    uint16_t limit; // Size of the IDT in bytes - 1
    uint32_t base;  // Base address of the IDT
} __attribute__((packed)) idt_ptr_t;

void set_idt_entry(int vector, void *handler);
void idt_init();

#endif
