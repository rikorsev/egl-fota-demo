#ifndef SLOT_H
#define SLOT_H

enum
{
    SLOT_BOOT = 0,
    SLOT_A,
    SLOT_B
};

#define SLOT_BOOT_ADDR plat_slot_boot_addr_get()
#define SLOT_A_ADDR plat_slot_a_addr_get()
#define SLOT_B_ADDR plat_slot_b_addr_get()

egl_value_u32_t *plat_slot_boot_addr_get(void);
egl_value_u32_t *plat_slot_a_addr_get(void);
egl_value_u32_t *plat_slot_b_addr_get(void);

#endif