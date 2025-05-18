#ifndef PLAT_RFM_RESET_PIN_H
#define PLAT_RFM_RESET_PIN_H

#define PLAT_RFM_RESET_PIN plat_rfm_reset_get()

egl_pio_t *plat_rfm_reset_get(void);

#endif