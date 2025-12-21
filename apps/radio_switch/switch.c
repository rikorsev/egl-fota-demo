#include "egl_lib.h"
#include "protocol.h"
#include "plat.h"
#include "switch.h"
#include "radio.h"

#define TASK_CLEAR(__task) (g_task &= ~__task)

static switch_task_t g_task;

static egl_result_t switch_task_local_state_toggle_handle(void)
{
    egl_result_t result;

    result = egl_pio_toggle(SYSLED);
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t switch_task_set(switch_task_t task)
{
    g_task |= task;

    return EGL_SUCCESS;
}

egl_result_t switch_task(void)
{
    egl_result_t result = EGL_SUCCESS;

    while(g_task)
    {
        if(g_task & SWITCH_TASK_LOCAL_STATE_TOGGLE)
        {
            EGL_LOG_DEBUG("handle: SWITCH_TASK_LOCAL_STATE_TOGGLE");
            TASK_CLEAR(SWITCH_TASK_LOCAL_STATE_TOGGLE);
            result = switch_task_local_state_toggle_handle();
            EGL_RESULT_CHECK(result);
        }
    }

    return result;
}