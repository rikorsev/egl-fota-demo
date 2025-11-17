#ifndef PLAT_RFM_H
#define PLAT_RFM_H

#include "egl_lib.h"

#if CONFIG_APP_TARGET_RFM_69
    #define PLAT_RFM69 plat_rfm69_get()
    egl_rfm69_t *plat_rfm69_get(void);
    void plat_rfm_dio2_irq_handler(void);
#elif CONFIG_APP_TARGET_RFM_66
    #define PLAT_RFM66 plat_rfm66_get()
    egl_rfm66_t *plat_rfm66_get(void);
    void plat_rfm_dio3_irq_handler(void);
#endif

#endif
