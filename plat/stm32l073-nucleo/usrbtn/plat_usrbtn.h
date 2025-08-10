#ifndef PLAT_USRBTN_H
#define PLAT_USRBTN_H

#include "egl_pio.h"

#define USER_BUTTON plat_usrbtn_get()

egl_pio_t *plat_usrbtn_get(void);
void plat_usrbtn_irq_handler(void);

#endif
