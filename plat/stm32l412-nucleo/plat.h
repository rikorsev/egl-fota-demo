#ifndef PLAT_H
#define PLAT_H

/* WORKAROUND: TBD resolve */
// #define STM32L412xx

#include "stm32l4xx.h"

#include "pio_led_green.h"

#define PLATFORM platform_get()

egl_platform_t *platform_get(void);

#endif