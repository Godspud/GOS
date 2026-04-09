#ifndef TIMER_H
#define TIMER_H

extern volatile unsigned int ticks;

void time_init();
void tick();
int get_time_ticks();

#endif
