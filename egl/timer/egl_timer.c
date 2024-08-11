#include <stdlib.h>

#include "egl_result.h"
#include "egl_timer.h"

egl_result_t egl_timer_init(egl_timer_t *timer)
{
    EGL_ASSERT_CHECK(timer);
    EGL_ASSERT_CHECK(timer->init);

    return timer->init();
}

uint32_t egl_timer_get(egl_timer_t *timer)
{
    if(timer == NULL)
    {
        return 0;
    }

    if(timer->get == NULL)
    {
        return 0;
    }

    return timer->get();
}

egl_result_t egl_timer_set(egl_timer_t *timer, uint32_t val)
{
    EGL_ASSERT_CHECK(timer);
    EGL_ASSERT_CHECK(timer->set);

    return timer->set(val);
}

egl_result_t egl_timer_deinit(egl_timer_t *timer)
{
    EGL_ASSERT_CHECK(timer);
    EGL_ASSERT_CHECK(timer->deinit);

    return timer->deinit();
}
