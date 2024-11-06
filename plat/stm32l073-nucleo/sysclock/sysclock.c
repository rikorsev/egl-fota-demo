#include "egl_lib.h"
#include "plat.h"

static uint32_t get(void)
{
    /* For current platform (at least for now) we are using MSI clock source,
       so, lets just calculate and return MSI clock value */

    uint32_t msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> RCC_ICSCR_MSIRANGE_Pos;
    return 32768U * (1U << (msirange + 1U));
}

static egl_clock_t plat_sysclock_inst =
{
    .get  = get
};

egl_clock_t *plat_sysclock_get(void)
{
    return &plat_sysclock_inst;
}
