#ifndef CMOS_H
#define CMOS_H

typedef struct
{
    unsigned char seconds;
    unsigned char minutes;
    unsigned char hours;
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char century;
} cmos_time;

int cmos_init(void);
void cmos_set_gmt_offset(int offset);
void cmos_get_time(cmos_time *time);

extern float gmt_offset;
extern cmos_time cmos_current_time;

#endif
