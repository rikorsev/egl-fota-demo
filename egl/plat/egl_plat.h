#ifndef EGL_PLAT_H
#define EGL_PLAT_H

#include <stdint.h>

#include "egl_result.h"

#pragma pack(push, 4)

typedef struct
{
    uint8_t major;
    uint8_t minor;
    uint8_t revision;
    char sufix[CONFIG_EGL_PLAT_INFO_VERSION_SUFIX_BUFF_SIZE];
}egl_plat_version_t;

typedef struct
{
    char branch[CONFIG_EGL_PLAT_INFO_GIT_BRANCH_SUFIX_BUFF_SIZE];
    char commit[CONFIG_EGL_PLAT_INFO_GIT_HASH_SUFIX_BUFF_SIZE];
}egl_git_t;

typedef struct
{
    uint32_t magic;
    uint32_t size;
    uint32_t checksum;
    char name[CONFIG_EGL_PLAT_INFO_NAME_BUFF_SIZE];
    char buildtime[CONFIG_EGL_PLAT_INFO_BUILDTIME_BUFF_SIZE];
    egl_plat_version_t version;
    egl_git_t git;
    uint32_t boot_number;
}egl_plat_info_t;

typedef struct
{
    egl_result_t    (*init)(void);
    egl_result_t    (*sleep)(uint32_t delay);
    egl_result_t    (*boot)(unsigned int slot_idx);
    egl_result_t    (*reboot)(void);
    egl_result_t    (*shutdown)(void);
    egl_plat_info_t *(*info)(void);
    egl_plat_info_t *(*slot_info)(unsigned int slot_idx);
    uint32_t        (*clock)(void);
}egl_platform_t;

#pragma pack(pop)

egl_result_t egl_plat_init(egl_platform_t *plat);
egl_result_t egl_plat_sleep(egl_platform_t *plat, uint32_t delay);
egl_result_t egl_plat_boot(egl_platform_t *plat, unsigned int slot_idx);
egl_result_t egl_plat_reboot(egl_platform_t *plat);
egl_result_t egl_plat_shutdown(egl_platform_t *plat);
egl_plat_info_t *egl_plat_info(egl_platform_t *plat);
egl_plat_info_t *egl_plat_slot_info(egl_platform_t *plat, unsigned int slot_idx);
uint32_t egl_plat_clock(egl_platform_t *plat);

#endif