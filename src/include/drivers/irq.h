#ifndef IRQ_H
#define IRQ_H

void send_eoi();
void irq_handler();
int irq_remap();

#endif