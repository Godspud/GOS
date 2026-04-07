#include "core/idt.h"
#include "include/drivers/irq.h"

extern void irq0_handler(void); // in asm
extern void irq_dummy(void);    // asm oso
extern void isr_dummy(void);    // in asm

static idt_entry_t idt[256];
static idt_ptr_t idtr;

void set_idt_entry(int vector, void *handler)
{
    unsigned int addr = (unsigned int)handler;
    idt[vector].isr_low = addr & 0xFFFF;
    idt[vector].kernel_cs = 0x08; // code segment from GDT
    idt[vector].reserved = 0;
    idt[vector].attributes = 0x8E; // Interrupt gate, present, ring 0
    idt[vector].isr_high = (addr >> 16) & 0xFFFF;
}

void idt_init()
{
    idtr.limit = sizeof(idt_entry_t) * 256 - 1;
    idtr.base = (unsigned int)&idt;

    // Handle CPU exceptions (0–31)
    for (int i = 0; i < 32; i++)
    {
        set_idt_entry(i, isr_dummy);
    }

    // Handle IRQs (32–255)
    for (int i = 32; i < 256; i++)
    {
        set_idt_entry(i, irq_dummy);
    }

    set_idt_entry(0x20, irq0_handler);

    __asm__ volatile("lidt %0" : : "m"(idtr));
}