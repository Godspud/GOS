# gdt.c

## Dependencies

[src/core/gdt.h](../core/gdt.h)

## Structs

### GDT Entry(gdt_entry)

**Attributes:**

- `packed`

**Feilds:**

- `uint16_t limit_low`
- `uing16_t base_low`
- `uint8_t base_mid`
- `uint8_t access`
- `uint8_t gran`
- `uint8_t base_high`<br/>
  Used for
    1. limit_low + gran(high part) to from 20bit segment lim
    2. base_low + base_mid + base_high 32bit base addr of the segment
    3. access present,privilage,type bits
    4. gran(low part) size

---

### Gdt Pointer(gdt_ptr)

**Attributes**

- `packed`

**Feilds**

- `uint16_t limit`
- `uint32_t base`<br/>
  used for
    1. limit size of GDT - 1
    2. base addr of the GDT array

## Functions

### `GDT Flush(gdt_flush)`

Note: loaded from [ASM](../../gdt.asm)<br/>
Type: void<br/>
takes a intalised Gdt Pointer and pushes it to the CPU

---

### `GDT Set Gate(gdt_set_gate)`

Type: void<br/>
used in preping for Gdt Init<br/>
**Parameters:**

- `int num`
- `uint32_t base`
- `uint32_t limit`
- `uint8_t access`
- `uint8_t gran`<br/>
  num used for deciding where to put the gdt entry<br/>
  base is used for the starting addr of the segment in memory<br/>
  limit is for the size of the segment<br/>
  access controls the type of segment<br/>
  gran controls the operand size<br/>

---

### `GDT init(gdt_init)`

Note: INIT func<br/>
Type: void<br/>
Inits the GDT for use
