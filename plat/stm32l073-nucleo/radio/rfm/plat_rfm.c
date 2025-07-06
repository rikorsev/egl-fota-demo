#include "egl_lib.h"
#include "plat_syspm.h"

extern const egl_pm_t plat_rfm_pm_inst;
extern const egl_iface_t plat_rfm_iface_inst;
extern const egl_pio_t plat_rfm_dio0_inst;
extern const egl_pio_t plat_rfm_dio1_inst;
extern const egl_pio_t plat_rfm_dio2_inst;
extern const egl_pio_t plat_rfm_dio3_inst;
extern const egl_pio_t plat_rfm_dio4_inst;
extern const egl_pio_t plat_rfm_dio5_inst;
extern const egl_clock_t plat_rfm_clock_inst;

#if CONFIG_APP_TARGET_RFM_69

const egl_rfm69_t plat_rfm69_inst =
{
    .pm    = (egl_pm_t *) &plat_rfm_pm_inst,
    .clock = (egl_clock_t *) &plat_rfm_clock_inst,
    .iface = (egl_iface_t *) &plat_rfm_iface_inst,
    .dio0  = (egl_pio_t *) &plat_rfm_dio0_inst,
    .dio1  = (egl_pio_t *) &plat_rfm_dio1_inst,
    .dio2  = (egl_pio_t *) &plat_rfm_dio2_inst,
    .dio3  = (egl_pio_t *) &plat_rfm_dio3_inst,
    .dio4  = (egl_pio_t *) &plat_rfm_dio4_inst,
    .dio5  = (egl_pio_t *) &plat_rfm_dio5_inst,
};

egl_rfm69_t *plat_rfm69_get(void)
{
    return (egl_rfm69_t *) &plat_rfm69_inst;
}

#elif CONFIG_APP_TARGET_RFM_66

const egl_rfm66_t plat_rfm66_inst =
{
    .pm    = (egl_pm_t *) &plat_rfm_pm_inst,
    .clock = (egl_clock_t *) &plat_rfm_clock_inst,
    .iface = (egl_interface_t *) &plat_rfm_iface_inst,
    .dio0  = (egl_pio_t *) &plat_rfm_dio0_inst,
    .dio1  = (egl_pio_t *) &plat_rfm_dio1_inst,
    .dio2  = (egl_pio_t *) &plat_rfm_dio2_inst,
    .dio3  = (egl_pio_t *) &plat_rfm_dio3_inst,
    .dio4  = (egl_pio_t *) &plat_rfm_dio4_inst,
    .dio5  = (egl_pio_t *) &plat_rfm_dio5_inst
};

egl_rfm66_t *plat_rfm66_get(void)
{
    return (egl_rfm66_t *) &plat_rfm66_inst;
}

#endif