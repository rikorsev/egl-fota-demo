/** @file The file is implementing FOTA manager that is responcible for performing of
 *        Firmware Update Over the Air with a radiomodule (RFM66).
 */

#ifndef FOTA_MGR_H
#define FOTA_MGR_H

/**
 * @brief Initialize FOTA manager
 *
 * Initialize hardware required to perform FOTA
 *
 * @return EGL_SUCCESS in case of success. Other EGL status otherwise
 */
egl_result_t fota_mgr_init(void);

/**
 * @brief Perform the FOTA process
 */
void fota_mgr_process(void);

#endif
