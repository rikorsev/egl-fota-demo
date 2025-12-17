#include "egl_lib.h"
#include "plat.h"

egl_result_t boot_mgr_process(void)
{
    egl_result_t result;
    plat_boot_config_t boot_config = { .slot = PLAT_SLOT_A };

    result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT, &boot_config, NULL);
    EGL_RESULT_CHECK(result);

    return result;
}
