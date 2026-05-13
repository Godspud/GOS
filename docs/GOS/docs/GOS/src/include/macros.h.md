# `macros.h`

## Externs

- `extern volatile unsigned int ticks`

## Macros

### Timing

- `every(ms)` `if (ticks % ms == 0)` Runs the following block every `ms` ticks
