#ifndef PLAT_H
#define PLAT_H

#include "stm32l0xx.h"

// #include "plat_crc.h" 
// #include "plat_flash.h"
// #include "plat_radio.h"

#include "sysclock.h"
#include "sysled.h"
#include "syslog.h"
#include "systimer.h"
#include "slot.h"

#define PLATFORM platform_get()
#define PLAT_NUM_SLOTS (3U)

enum
{
    PLAT_SLOT_BOOT,
    PLAT_SLOT_A,
    PLAT_SLOT_B
};

egl_platform_t *platform_get(void);

#endif