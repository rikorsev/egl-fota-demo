#include "egl_lib.h"

#define PLAT_RFM69_XTAL_CLOCK_FREQ_HZ (32000000U)

static egl_result_t init(void)
{
    return EGL_SUCCESS;
}

static uint32_t get(void)
{
    return PLAT_RFM69_XTAL_CLOCK_FREQ_HZ;
}

const egl_clock_t plat_rfm_clock_inst =
{
    .init = init,
    .get = get
};