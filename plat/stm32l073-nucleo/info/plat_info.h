#ifndef PLAT_INFO_H
#define PLAT_INFO_H

#include "egl_lib.h"

#define CONFIG_APP_INFO_VERSION_SUFIX_BUFF_SIZE     (16U)
#define CONFIG_APP_INFO_GIT_BRANCH_BUFF_SIZE        (32U)
#define CONFIG_APP_INFO_GIT_HASH_BUFF_SIZE          (41U)
#define CONFIG_APP_INFO_NAME_BUFF_SIZE              (32U)
#define CONFIG_APP_INFO_BUILDTIME_BUFF_SIZE         (64U)

#pragma pack(push, 4)

typedef struct
{
    uint8_t major;
    uint8_t minor;
    uint8_t revision;
    char sufix[CONFIG_APP_INFO_VERSION_SUFIX_BUFF_SIZE];
}app_version_t;

typedef struct
{
    char branch[CONFIG_APP_INFO_GIT_BRANCH_BUFF_SIZE];
    char commit[CONFIG_APP_INFO_GIT_HASH_BUFF_SIZE];
}app_git_info_t;

typedef struct
{
    uint32_t magic;
    uint32_t size;
    uint32_t checksum;
    char name[CONFIG_APP_INFO_NAME_BUFF_SIZE];
    char buildtime[CONFIG_APP_INFO_BUILDTIME_BUFF_SIZE];
    app_version_t version;
    app_git_info_t git;
    uint32_t boot_number;
}app_info_t;

#pragma pack(pop)

#define APP_INFO plat_app_info_ptr_get()
egl_value_ptr_t *plat_app_info_ptr_get(void);

#endif