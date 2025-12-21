#ifndef FOTA_MGR_H
#define FOTA_MGR_H

#include "egl_lib.h"
#include "plat.h"

typedef enum
{
    FOTA_TASK_DOWNLOAD,
    FOTA_TASK_UPLOAD
}fota_task_t;

egl_result_t fota_process(plat_boot_slot_t target_slot, fota_task_t task);

#endif