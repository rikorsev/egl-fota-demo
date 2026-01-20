#include "egl_lib.h"
#include "protocol.h"
#include "plat.h"
#include "switch.h"
#include "radio.h"

static void *thread_handle = NULL;
static void *flags_handle = NULL;

static void switch_thread_entry(void *param)
{
    egl_result_t result;
    unsigned int flags = 0;

    while(1)
    {
        result = egl_os_flags_wait(SYSOS, flags_handle, SWITCH_TOGGLE_FLAG, &flags,
                                   EGL_OS_FLAGS_OPT_WAIT_ANY, EGL_OS_WAIT_FOREWER);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);

        if(flags & SWITCH_TOGGLE_FLAG)
        {
            result = egl_pio_toggle(SYSLED);
            EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
        }
    }
}

egl_result_t switch_flag_set(unsigned int flag)
{
    egl_result_t result;

    result = egl_os_flags_set(SYSOS, flags_handle, flag);
    EGL_RESULT_CHECK(result);

    return EGL_SUCCESS;
}

egl_result_t switch_init(void)
{
    egl_result_t result;

    static egl_os_flags_ctx flags_ctx;
    static egl_os_thread_ctx thread_ctx;
    static uint8_t stack[1024];

    result = egl_pio_init(SYSLED);
    EGL_RESULT_CHECK(result);

    result = egl_pio_set(SYSLED, false);
    EGL_RESULT_CHECK(result);

    result = egl_os_flags_create(SYSOS, &flags_handle, "SwitchFlags", &flags_ctx);
    EGL_RESULT_CHECK(result);

    result = egl_os_thread_create(SYSOS, &thread_handle, "Switch",
                                  switch_thread_entry, NULL,
                                  stack, sizeof(stack),
                                  1, &thread_ctx);
    EGL_RESULT_CHECK(result);

    return result;
}
