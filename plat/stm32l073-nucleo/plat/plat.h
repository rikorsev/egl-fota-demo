#ifndef PLAT_H
#define PLAT_H

#include "stm32l0xx.h"

#include "plat_sysclock.h"
#include "plat_sysled.h"
#include "plat_syslog.h"
#include "plat_systimer.h"
#include "plat_syspm.h"
#include "plat_system.h"
#include "plat_info.h"
#include "plat_usrbtn.h"
#include "plat_crc.h"
#include "plat_flash.h"
#include "slot.h"

#if CONFIG_APP_TARGET_RFM_69 || CONFIG_APP_TARGET_RFM_66
#include "plat_rfm.h"
#include "plat_radio_iface.h"
#endif

#define PLATFORM platform_get()
#define PLAT_NUM_SLOTS (3U)

typedef enum
{
    PLAT_SLOT_BOOT,
    PLAT_SLOT_A,
    PLAT_SLOT_B
}plat_boot_slot_t;

enum
{
    PLAT_CMD_BOOT,
    PLAT_CMD_BOOT_CONFIG_GET
};

typedef enum
{
    PLAT_FOTA_TASK_NONE,
    PLAT_FOTA_TASK_UPLOAD_SLOT_A,
    PLAT_FOTA_TASK_UPLOAD_SLOT_B,
    PLAT_FOTA_TASK_DOWNLOAD_SLOT_A,
    PLAT_FOTA_TASK_DOWNLOAD_SLOT_B
}plat_fota_task_t;

typedef struct
{
    plat_boot_slot_t slot;
    plat_fota_task_t task;
    uint32_t checksum;
}plat_boot_config_t;

egl_platform_t *platform_get(void);

#endif