#include <string.h>
#include "egl_lib.h"
#include "plat.h"

#define PLAT_FLASH_CHUNK_SIZE (128U)

static egl_result_t init(void)
{
    RCC->AHBENR |= RCC_AHBENR_MIFEN;

    return EGL_SUCCESS;
}

static egl_result_t read(void *addr, void *data)
{
    EGL_LOG_DEBUG("flash read page. Addr: %08p", addr);

    memcpy(data, addr, PLAT_FLASH_PAGE_SIZE);

    return EGL_SUCCESS;
}

inline static void wait_for_ready(void)
{
    while (FLASH->SR & FLASH_SR_BSY);
}

static egl_result_t unlock(void)
{
    #define FLASH_PE_KEY1 (0x89ABCDEF)
    #define FLASH_PE_KEY2 (0x02030405)
    #define FLASH_PG_KEY1 (0x8C9DAEBF)
    #define FLASH_PG_KEY2 (0x13141516)

    // #define FLASH_KEY1 (0x45670123)
    // #define FLASH_KEY2 (0xCDEF89AB)

    if (FLASH->PECR & FLASH_PECR_PELOCK)
    {
        FLASH->PEKEYR = FLASH_PE_KEY1;
        FLASH->PEKEYR = FLASH_PE_KEY2;
    }

    if (FLASH->PECR & FLASH_PECR_PRGLOCK)
    {
        FLASH->PRGKEYR = FLASH_PG_KEY1;
        FLASH->PRGKEYR = FLASH_PG_KEY2;
    }

    return EGL_SUCCESS;
}

static egl_result_t lock(void)
{
    FLASH->PECR |= FLASH_PECR_PRGLOCK;
    FLASH->PECR |= FLASH_PECR_PELOCK;

    return EGL_SUCCESS;
}

static egl_result_t erase(void *addr)
{
    FLASH->PECR |= FLASH_PECR_ERASE;
    FLASH->PECR |= FLASH_PECR_PROG;

    *(volatile uint32_t*)addr = 0;

    wait_for_ready();

    FLASH->PECR &= ~(FLASH_PECR_ERASE | FLASH_PECR_PROG);

    return EGL_SUCCESS;
}

static egl_result_t programm(void *addr, void *data)
{
    volatile uint32_t *addr_ptr = addr;
    uint32_t databuf[2];

    for(uint32_t i = 0; i < PLAT_FLASH_CHUNK_SIZE; i++)
    {
        memcpy(databuf, data, sizeof(databuf));

        wait_for_ready();

        FLASH->PECR |= FLASH_PECR_PROG;

        *addr_ptr = databuf[0];
        addr_ptr++;

        *addr_ptr = databuf[1];
        addr_ptr++;

        data = (uint8_t *)data + sizeof(databuf);
    }

    return EGL_SUCCESS;
}

static egl_result_t write(void *addr, void *data)
{
    egl_result_t result;
    EGL_LOG_DEBUG("flash write page. Addr: %08x", addr);

    /* Disable interrupts to avoid any interruption during the loop */
    uint32_t primask_bit = __get_PRIMASK();
    __disable_irq();

    wait_for_ready();

    result = unlock();
    EGL_RESULT_CHECK_EXIT(result);

    for(unsigned int i = 0; i < PLAT_FLASH_PAGE_SIZE / PLAT_FLASH_CHUNK_SIZE; i++)
    {
        result = erase(addr);
        EGL_RESULT_CHECK_EXIT(result);

        result = programm(addr, data);
        EGL_RESULT_CHECK_EXIT(result);

        addr += PLAT_FLASH_CHUNK_SIZE;
        data = (uint8_t *)data + PLAT_FLASH_CHUNK_SIZE;
    }

    result = lock();
    EGL_RESULT_CHECK_EXIT(result);

exit:
    /* Re-enable the interrupts */
    __set_PRIMASK(primask_bit);

    return result;
}

static const egl_block_t egl_block_flash_inst =
{
    .init = init,
    .read = read,
    .write = write
};

egl_block_t *egl_block_flash_get(void)
{
    return (egl_block_t *)&egl_block_flash_inst;
}