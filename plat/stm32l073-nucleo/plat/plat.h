#ifndef PLAT_H
#define PLAT_H

#include "stm32l0xx.h"

// #include "plat_crc.h" 
// #include "plat_flash.h"

#include "plat_sysclock.h"
#include "plat_sysled.h"
#include "plat_syslog.h"
#include "plat_systimer.h"
#include "plat_syspm.h"
#include "plat_rfm.h"
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