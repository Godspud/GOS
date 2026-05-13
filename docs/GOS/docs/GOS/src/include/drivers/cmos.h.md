# `cmos.h`

## Structs

### CMOS Time(`cmos_time`)

**Fields:**

- `unsigned char seconds`
- `unsigned char minutes`
- `unsigned char hours`
- `unsigned char day`
- `unsigned char month`
- `unsigned char year`
- `unsigned char century`

## Externs

- `extern float gmt_offset`
- `extern cmos_time cmos_current_time`

## Functions

Function declaration for

1. [cmos_init](../../drivers/cmos.c#void-cmos_initvoid)
2. [cmos_set_gmt_offset](../../drivers/cmos.c#void-cmos_set_gmt_offsetint-offset)
3. [cmos_get_time](../../drivers/cmos.c#void-cmos_get_timecmos_time-time)
