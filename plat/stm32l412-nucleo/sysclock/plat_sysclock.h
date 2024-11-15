#ifndef PLAT_SYSCLOCK_H
#define PLAT_SYSCLOCK_H

#define SYSCLOCK plat_sysclock_get()

egl_clock_t *plat_sysclock_get(void);

#endif
