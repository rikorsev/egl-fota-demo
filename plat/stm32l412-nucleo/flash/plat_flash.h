#ifndef PLAT_FLASH_H
#define PLAT_FLASH_H

#define PLAT_FLASH_PAGE_SIZE (2048U)
#define PLAT_FLASH egl_block_flash_get()

egl_block_t *egl_block_flash_get(void);

#endif