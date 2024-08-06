#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "egl_result.h"
#include "egl_pio.h"

egl_result_t egl_pio_init(egl_pio_t *pio)
{
    EGL_ASSERT_CHECK(pio);
    EGL_ASSERT_CHECK(pio->init);

    return pio->init();
}

egl_result_t egl_pio_set(egl_pio_t *pio, bool state)
{
    EGL_ASSERT_CHECK(pio);
    EGL_ASSERT_CHECK(pio->set);

    return pio->set(state);
}

egl_result_t egl_pio_get(egl_pio_t *pio)
{
    EGL_ASSERT_CHECK(pio);
    EGL_ASSERT_CHECK(pio->get);

    return pio->get();
}

egl_result_t egl_pio_toggle(egl_pio_t *pio)
{
    EGL_ASSERT_CHECK(pio);

    egl_result_t state = egl_pio_get(pio);

    if(state == EGL_SET)
    {
        state = egl_pio_set(pio, false);
    }
    else if(state == EGL_RESET)
    {
        state = egl_pio_set(pio, true);
    }

    return state;
}

egl_result_t egl_pio_callback_set(egl_pio_t *pio, egl_pio_callback_t callback)
{
    EGL_ASSERT_CHECK(pio);

    pio->callback = callback;

    return EGL_SUCCESS;
}

egl_result_t egl_pio_deinit(egl_pio_t *pio)
{
    EGL_ASSERT_CHECK(pio);
    EGL_ASSERT_CHECK(pio->deinit);

    return pio->deinit();
}
