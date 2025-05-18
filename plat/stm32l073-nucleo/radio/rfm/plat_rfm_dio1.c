#include "egl_lib.h"

static egl_result_t init(void)
{
    return EGL_SUCCESS;
}

const egl_pio_t plat_rfm_dio1_inst =
{
    .init = init,
};
