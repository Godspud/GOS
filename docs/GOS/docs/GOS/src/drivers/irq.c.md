# `irq.c`

## Dependencies

[include/drivers/time/timer.h](../include/drivers/time/timer.h)<br/>
[include/drivers/io.h](../include/drivers/io.h)<br/>
[include/drivers/keyboard.h](../include/drivers/keyboard.h)<br/>

## Functions

### `IRQ handler(irq_handler)`

Type: void<br/>
handles any and all irq(_only 32(pit) currently supported_)<br/>
**Parameters:**

- `int irq`<br/>
  the irq no to be handelled

---

### `IO Wait(io_wait)`

Type: void<br/>
TODO: this

---

### `IRQ remap(irq_remap)`

Type: void
hindes and changes all IRQ's to a better spot then unhides the PIC IRQ
