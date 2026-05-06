# random.c

## Functions

### `Random No.(rand)`

**Returns:**

- random no based on fomula ((next \* 1103515245 + 12345) / 65536) % 32768 where next starts at 19752

### `Seeded random No.(srand)`

**Parameters:**

- `unsigned int seed`
  seed is used to set the next for the [rand func](#random-norand)

**Returns:**

- random no with seeded fomula
