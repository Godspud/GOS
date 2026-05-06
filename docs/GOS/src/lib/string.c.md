# `src/lib/string.c`

## Dependencies

None

## Functions

### `int strlen(const char *str)`

**Parameters:**
- `const char *str`

**Returns:**
- `len` → len

---

### `int strcmp(const char *s1, const char *s2)`

**Parameters:**
- `const char *s1`
- `const char *s2`

**Returns:**
- `*(unsigned char *)s1 - *(unsigned char *)s2` → *(unsigned char *)s1 - *(unsigned char *)s2

---

### `int strncmp(const char *s1, const char *s2, int n)`

**Parameters:**
- `const char *s1`
- `const char *s2`
- `int n`

**Returns:**
- `0` → 0
- `*(unsigned char *)s1 - *(unsigned char *)s2` → *(unsigned char *)s1 - *(unsigned char *)s2

---

### `void memset(void *ptr, int value, int n)`

**Parameters:**
- `void *ptr`
- `int value`
- `int n`

**Returns:** nothing / void

---

### `void memcpy(void *dest, const void *src, int n)`

**Parameters:**
- `void *dest`
- `const void *src`
- `int n`

**Returns:** nothing / void

---

### `static void str_copy(char *dest, const char *src, int max)`

**Parameters:**
- `char *dest`
- `const char *src`
- `int max`

**Returns:** nothing / void

---

### `void strreplace(const char *src, char *dest, char old_char, char new_char)`

**Parameters:**
- `const char *src`
- `char *dest`
- `char old_char`
- `char new_char`

**Returns:** nothing / void

---

### `char *strncpy(char *dest, const char *src, unsigned int n)`

**Parameters:** none

**Returns:**
- `dest` → dest

---

### `void int_to_str(int num_to_convert, char *str)`

**Parameters:**
- `int num_to_convert`
- `char *str`

**Returns:**
- `void` → void return

