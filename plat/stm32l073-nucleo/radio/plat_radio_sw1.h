#ifndef PLAT_RADIO_SW1_H
#define PLAT_RADIO_SW1_H

#include "egl_pio.h"

#define RADIO_SW1 plat_radio_sw1_get()

egl_pio_t *plat_radio_sw1_get(void);
void plat_radio_sw1_irq_handler(void);

#endif