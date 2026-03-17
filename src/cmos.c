#include "cmos.h"
#include "io.h"

cmos_time cmos_current_time;

void cmos_init(void)
{
    // Nothing to initialize for CMOS
}

/**
 * cmos_get_time: Reads the current time from the CMOS registers and fills the provided cmos_time structure with the values. The function reads the seconds, minutes, hours, day, month, year, and century from their respective CMOS registers (0x00 to 0x09 and 0x32) and stores them in the cmos_time structure pointed to by the time parameter.
 */
void cmos_get_time(cmos_time *time)
{
    // Read time values from CMOS registers found at https://osdev.wiki/wiki/%601%60#Getting_Current_Date_and_Time_from_RTC
    time->seconds = inb(0x00);
    time->minutes = inb(0x02);
    time->hours = inb(0x04);
    time->day = inb(0x07);
    time->month = inb(0x08);
    time->year = inb(0x09);
    time->century = inb(0x32);
}
