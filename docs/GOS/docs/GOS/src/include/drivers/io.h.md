# `io.h`

## Functions

### `In Byte(inb)`

Note: static inline<br/>
Type: unsigned char<br/>
Reads a byte from the specified I/O port<br/>
**Parameters:**

- `unsigned short port`

**Returns:**

- `result` → byte read from port

---

### `Out Byte(outb)`

Note: static inline<br/>
Type: void<br/>
Writes a byte to the specified I/O port<br/>
**Parameters:**

- `unsigned short port`
- `unsigned char data`

---

### `In Word(inw)`

Note: static inline<br/>
Type: unsigned short<br/>
Reads a word (2 bytes) from the specified I/O port<br/>
**Parameters:**

- `unsigned short port`

**Returns:**

- `result` → word read from port

---

### `Out Word(outw)`

Note: static inline<br/>
Type: void<br/>
Writes a word (2 bytes) to the specified I/O port<br/>
**Parameters:**

- `unsigned short port`
- `unsigned short data`
