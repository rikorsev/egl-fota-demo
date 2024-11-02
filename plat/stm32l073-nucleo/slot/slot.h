#ifndef SLOT_H
#define SLOT_H

#define CONFIG_SLOT_INFO_VERSION_SUFIX_BUFF_SIZE     (16U)
#define CONFIG_SLOT_INFO_GIT_BRANCH_BUFF_SIZE        (32U)
#define CONFIG_SLOT_INFO_GIT_HASH_BUFF_SIZE          (41U)
#define CONFIG_SLOT_INFO_NAME_BUFF_SIZE              (32U)
#define CONFIG_SLOT_INFO_BUILDTIME_BUFF_SIZE         (64U)

#pragma pack(push, 4)

typedef struct
{
    uint8_t major;
    uint8_t minor;
    uint8_t revision;
    char sufix[CONFIG_SLOT_INFO_VERSION_SUFIX_BUFF_SIZE];
}slot_version_t;

typedef struct
{
    char branch[CONFIG_SLOT_INFO_GIT_BRANCH_BUFF_SIZE];
    char commit[CONFIG_SLOT_INFO_GIT_HASH_BUFF_SIZE];
}slot_git_t;

typedef struct
{
    uint32_t magic;
    uint32_t size;
    uint32_t checksum;
    char name[CONFIG_SLOT_INFO_NAME_BUFF_SIZE];
    char buildtime[CONFIG_SLOT_INFO_BUILDTIME_BUFF_SIZE];
    slot_version_t version;
    slot_git_t git;
    uint32_t boot_number;
}slot_info_t;

#pragma pack(pop)

const slot_info_t *slot_info_get(void);
const uint32_t slot_addr_get(unsigned int slot_idx);

#endif