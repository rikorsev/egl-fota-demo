#ifndef SYSTIMER_H
#define SYSTIMER_H

#include "egl_timer.h"

#define SYSTIMER plat_systimer_get()

void plat_systimer_irq_handler(void);
egl_timer_t *plat_systimer_get(void);

#endif