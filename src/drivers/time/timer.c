#include "include/drivers/cmos.h"

int current_time = 0;
int years = 0;
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
int ticks = 0;
cmos_time time;

void timer_init()
{
    cmos_get_time(&time);
    years = time.year + time.century * 100;
    // IMPLEMENT LEAP YEARS
    // TODO: IMPLENT LEAP YEARS
    days = time.day + years * 365;
    hours = time.hours + days * 24;
    minutes = time.minutes + hours * 60;
    seconds = time.seconds + minutes * 60;
    ticks = seconds * 1000;
}

int tick()
{
    ticks += 1;
}

int get_time_ticks()
{
    return ticks;
}
