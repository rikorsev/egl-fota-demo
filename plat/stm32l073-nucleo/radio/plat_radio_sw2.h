#ifndef PLAT_RADIO_SW2_H
#define PLAT_RADIO_SW2_H

#include "egl_pio.h"

#define RADIO_SW2 plat_radio_sw2_get()

egl_pio_t *plat_radio_sw2_get(void);
void plat_radio_sw2_irq_handler(void);

#endif