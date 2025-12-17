#ifndef SLOT_H
#define SLOT_H

#define SLOT_BOOT_ADDR plat_slot_boot_addr_get()
#define SLOT_A_ADDR plat_slot_a_addr_get()
#define SLOT_B_ADDR plat_slot_b_addr_get()

#define SLOT_BOOT_INFO plat_slot_boot_info_get()
#define SLOT_A_INFO plat_slot_a_info_get()
#define SLOT_B_INFO plat_slot_b_info_get()

egl_value_u32_t *plat_slot_boot_addr_get(void);
egl_value_u32_t *plat_slot_a_addr_get(void);
egl_value_u32_t *plat_slot_b_addr_get(void);

egl_value_ptr_t *plat_slot_boot_info_get(void);
egl_value_ptr_t *plat_slot_a_info_get(void);
egl_value_ptr_t *plat_slot_b_info_get(void);

#endif