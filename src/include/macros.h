#ifndef MACROS_H
#define MACROS_H

extern volatile unsigned int ticks;

#define every(ms) if (ticks % ms == 0)

#endif