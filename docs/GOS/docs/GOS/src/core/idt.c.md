# idt.c

## Dependencies

[src/core/idt.h](idt.h)<br/>
[src/include/drivers/irq.h](../include/drivers/irq.h)

## Functions

### `IRQ0 handler(irq0_handler)`

Note: loaded from [ASM](../../irq.asm)<br/>
Type: void<br/>
the interupt handler for [PIT](../drivers/time/pit.c)

---

### `IRQ Dummy(irq_dummy)`

Note: loaded from [ASM](../../irq.asm)<br/>
Type: void<br/>
the interupt handler for unused interupts

---

### `ISR Dummy(isr_dummy)`

Note: loaded from [ASM](../../irq.asm)<br/>
Type: void<br/>
the cpu exception handler([TODO](../../TODO.md): change to smt where it prints the execption)

---

### `IRQ1 handler (irq1_handler)`

Note: loaded from [ASM](../../irq.asm)<br/>
Type: void<br/>
the keyboard interupt handler

---

### `Set IDT entry(set_idt_entry)`

Type: void<br/>
**Parameters:**

- `int vector`
- `void *handler`<br/>
  vertor used for deciding which index to put the entry<br/>
  \*handler used for finding the handler

---

### `IDT init(idt_init)`

Note: INIT func<br/>
Type: void<br/>
Inits the IDT for use
