#ifndef PLAT_H
#define PLAT_H

#include "stm32l4xx.h"

#include "pio_led_green.h"
#include "iface_trace.h"
#include "timer_sys.h"

#define PLATFORM platform_get()

egl_platform_t *platform_get(void);

#endif