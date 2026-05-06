# idt.h

## Dependencies

stdint.h(standerd lib)

## Structs

### IDT Entry(idt_entry_t)

**Attributes:**

- `packed`

**Feilds**

- `uint16_t isr_low`
- `uint16_t kernel_cs`
- `uint8_t reserved`
- `uint8_t attributes`
- `uint16_t isr_high`<br>
  Used for
    1. isr_low + isr_high to form 32bit bit handler func addr
    2. kernel_cs kernel code segment selector
    3. attributes types and attributes

### IDT Pointer(idt_ptr_t)

**Attributes:**

- `packed`

**Feilds**

- `uint16_t limit`
- `uint32_t base`<br>
  Used for
    1. limit size of IDT - 1
    2. base addr of the IDT array
