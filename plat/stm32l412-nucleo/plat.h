#ifndef PLAT_H
#define PLAT_H

#include "stm32l4xx.h"

#include "plat_info.h"
#include "pio_led_green.h"
#include "iface_trace.h"
#include "timer_sys.h"

#define PLATFORM platform_get()

#define CONFIG_PLAT_META_SECTION_SIZE (2048U)

enum
{
    PLAT_SLOT_BOOT,
    PLAT_SLOT_A,
    PLAT_SLOT_B
};

egl_platform_t *platform_get(void);

#endif