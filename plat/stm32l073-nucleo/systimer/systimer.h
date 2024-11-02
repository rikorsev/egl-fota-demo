#ifndef SYSTIMER_H
#define SYSTIMER_H

#define SYSTIMER plat_systimer_get()

egl_timer_t *plat_systimer_get(void);

#endif