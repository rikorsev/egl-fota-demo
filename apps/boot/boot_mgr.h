/** @file The file is implementing Boot manager which is responcible for analyzing,
 *        validation and booting of specific slot
 */

#ifndef BOOT_MGR_H
#define BOOT_MGR_H

/**
 * @brief Initialize boot manager
 *
 * Readout slot info and sort it in slot table
 *
 * @return EGL_SUCCESS in case of success. Other EGL status otherwise
 */
egl_result_t boot_mgr_init(void);

/**
 * @brief Perform booting of first valid slot
 *
 * Perform validation in order according to slot table. As soon as slot be valid, perform booting
 * from that slot
 */
void boot_mgr_process(void);

/**
 * @brief Perform validation of specific slot
 *
 * @param info - Pointer to slot to validate
 *
 * @return EGL_SUCCESS if slot is valid
 */
egl_result_t boot_mgr_slot_validate(egl_plat_info_t *info);

/**
 * @brief Provides highest boot number from all slots on device
 *
 * @return highest boot number value
 */
uint32_t boot_mgr_highest_boot_number_get(void);

#endif