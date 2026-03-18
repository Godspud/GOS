#include "include/drivers/cmos.h"
#include "include/drivers/io.h"
#include "vga.h"
#define CURRENT_YEAR 2023

float gmt_offset = 8;
cmos_time cmos_current_time;

void cmos_init(void)
{
    // cmos_set_gmt_offset();
}
unsigned char cmos_read(unsigned char reg)
{
    outb(0x70, reg);
    return inb(0x71);
}
static unsigned char bcd_to_bin(unsigned char bcd)
{
    return ((bcd & 0x0F) + ((bcd >> 4) * 10));
}
/**
 * cmos_set_gmt_offset: Sets the GMT offset for time calculations. The offset is specified in hours and is used to adjust the time read from the CMOS to the user's local time zone. For example, if the user is in a time zone that is GMT+2, they would call cmos_set_gmt_offset(2) to set the offset to 2 hours ahead of GMT.
 */
void cmos_set_gmt_offset(int offset)
{
    gmt_offset = offset;
}
/**
 * cmos_get_time: Reads the current time from the CMOS registers and fills the provided cmos_time structure with the values. The function reads the seconds, minutes, hours, day, month, year, and century from their respective CMOS registers (0x00 to 0x09 and 0x32) and stores them in the cmos_time structure pointed to by the time parameter.
 */
void cmos_get_time(cmos_time *time)
{
    // Read time values from CMOS registers found at https://osdev.wiki/wiki/%601%60#Getting_Current_Date_and_Time_from_RTC
    // used code from the same wki page
    unsigned char century;
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char registerB;
    // Wait until the CMOS is not updating to ensure we get a consistent time reading
    while (cmos_read(0x0A) & 0x80)
        ;
    second = cmos_read(0x00);
    minute = cmos_read(0x02);
    hour = cmos_read(0x04);
    day = cmos_read(0x07);
    month = cmos_read(0x08);
    year = cmos_read(0x09);
    century = cmos_read(0x32);
    registerB = cmos_read(0x0B);
    if (!(registerB & 0x04))
    {
        second = bcd_to_bin(second);
        minute = bcd_to_bin(minute);
        // Mask out the 12/24 hour bit
        hour = bcd_to_bin(hour & 0x7F);
        day = bcd_to_bin(day);
        month = bcd_to_bin(month);
        year = bcd_to_bin(year);
    }
    if (!(registerB & 0x02) && (hour & 0x80))

        hour = ((hour & 0x7F) + 12) % 24;

    if (century != 0)

        year += century * 100;

    else

        year += (CURRENT_YEAR / 100) * 100;
    if (year < CURRENT_YEAR)
        year += 100;

    if (second == minute && minute == hour)
    {
        print_string("Warning: CMOS time may be inconsistent (seconds, minutes, and hours equvalent). Consider checking the CMOS battery.\n", COLOR_LIGHT_RED);
    }
    // switch ti users time zone
    hour = hour + gmt_offset;

    time->seconds = second;
    time->minutes = minute;
    time->hours = hour;
    time->day = day;
    time->month = month;
    time->year = year;
    time->century = century;
}
