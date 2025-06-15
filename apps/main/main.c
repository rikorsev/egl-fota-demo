#include "egl_lib.h"
#include "plat.h"
#include "rfm_test.h"

static egl_result_t error_hook_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static egl_result_t init(void)
{
    egl_result_t result;
    egl_result_error_hook_t error_hook = { error_hook_func };

    result = egl_system_init(SYSTEM);
    EGL_RESULT_CHECK(result);

    egl_result_error_hook_set(&error_hook);

    result = egl_pio_init(SYSLED);
    EGL_RESULT_CHECK(result);

#if CONFIG_RFM_TEST_ENABLED
    result = rfm_test_init();
    EGL_RESULT_CHECK(result);
#endif

    return result;
}

static egl_result_t info(void)
{
    egl_result_t result = EGL_SUCCESS;

#if CONFIG_EGL_LOG_ENABLED
    app_info_t *info = NULL;
    uint32_t slot_boot_addr;
    uint32_t slot_a_addr;
    uint32_t slot_b_addr;

    result = egl_value_ptr_get(APP_INFO, (void **)&info);
    EGL_RESULT_CHECK(result);
    EGL_ASSERT_CHECK(info, EGL_NOT_SUPPORTED);

    EGL_LOG_INFO("Application %s (%u.%u.%u%s) started", info->name,
                                                        info->version.major,
                                                        info->version.minor,
                                                        info->version.revision,
                                                        info->version.sufix);
    EGL_LOG_INFO("Date: %s", info->buildtime);
    EGL_LOG_INFO("Size: %u", info->size);
    EGL_LOG_INFO("Checksum: %08x", info->checksum);

    result = egl_value_u32_get(SLOT_BOOT_ADDR, &slot_boot_addr);
    EGL_RESULT_CHECK(result);

    result = egl_value_u32_get(SLOT_A_ADDR, &slot_a_addr);
    EGL_RESULT_CHECK(result);

    result = egl_value_u32_get(SLOT_B_ADDR, &slot_b_addr);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Boot address: 0x%08x", slot_boot_addr);
    EGL_LOG_INFO("Slot A address: 0x%08x", slot_a_addr);
    EGL_LOG_INFO("Slot B address: 0x%08x", slot_b_addr);
#endif

    return result;
}

static void blink(void)
{
    egl_pio_set(SYSLED, true);
    egl_sys_delay(50);
    egl_pio_set(SYSLED, false);
    egl_sys_delay(950);

    EGL_LOG_INFO("Tick...");
}

static void loop(void)
{
    blink();

#if CONFIG_RFM_TEST_ENABLED
    rfm_test_run();
#endif
}

int main(void)
{
    egl_result_t result = init();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_FAIL("Fatal error");
        EGL_RESULT_FATAL();
    }

    result = info();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_FAIL("Fatal error");
        EGL_RESULT_FATAL();
    }

    while(1)
    {
        loop();
    }

    return 0;
}