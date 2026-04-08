#include "core/idt.h"
#include "include/drivers/irq.h"
#include "vga.h"

extern void irq0_handler(void); // in asm
extern void irq_dummy(void);    // asm oso
extern void isr_dummy(void);    // in asm
extern void irq1_handler(void); // in asm

static idt_entry_t idt[256];
static idt_ptr_t idtr;

void print_hex(uint32_t val)
{
    char hex[] = "0123456789ABCDEF";
    char buf[9];
    buf[8] = '\0';

    for (int i = 7; i >= 0; i--)
    {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }

    print_string(buf, COLOR_LIGHT_RED);
}

void set_idt_entry(int vector, void *handler)
{
    unsigned int addr = (unsigned int)handler;
    idt[vector].isr_low = addr & 0xFFFF;
    idt[vector].kernel_cs = 0x08; // code segment from GDT
    idt[vector].reserved = 0;
    idt[vector].attributes = 0x8E; // Interrupt gate, present, ring 0
    idt[vector].isr_high = (addr >> 16) & 0xFFFF;
    print_hex(handler);
}

extern void idt_load(idt_ptr_t *);

void idt_init()
{
    idtr.limit = sizeof(idt_entry_t) * 256 - 1;
    idtr.base = (unsigned int)&idt;

    vga_write_string(0, 1, "IDT base: ", COLOR_LIGHT_RED);
    print_hex(idtr.base);

    vga_write_string(1, 0, "IDT limit: ", COLOR_LIGHT_RED);
    print_hex(idtr.limit);

    vga_write_string(2, 0, "idt_entry size: ", COLOR_LIGHT_RED);
    print_char('\n', COLOR_LIGHT_RED);
    print_hex(sizeof(idt_entry_t));

    for (int i = 0; i < 32; i++)
        set_idt_entry(i, isr_dummy);

    for (int i = 32; i < 256; i++)
        set_idt_entry(i, irq_dummy);

    set_idt_entry(0x20, irq0_handler);
    set_idt_entry(0x21, irq1_handler);

    idt_load(&idtr);
}