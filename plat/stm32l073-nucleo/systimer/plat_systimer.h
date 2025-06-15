#ifndef PLAT_SYSTIMER_H
#define PLAT_SYSTIMER_H

#include "egl_timer.h"

void plat_systimer_irq_handler(void);
egl_timer_t *plat_systimer_get(void);

#endif