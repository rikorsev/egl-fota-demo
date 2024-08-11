#ifndef BOOT_MGR_H
#define BOOT_MGR_H

egl_result_t boot_mgr_init(void);
void boot_mgr_process(void);
egl_result_t boot_mgr_slot_validate(egl_plat_info_t *info);
uint32_t boot_mgr_highest_boot_number_get(void);

#endif