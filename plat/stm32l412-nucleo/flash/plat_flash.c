#include <string.h>
#include "egl_lib.h"
#include "plat.h"

static egl_result_t init(void)
{
    return EGL_SUCCESS;
}

static egl_result_t read(uint32_t addr, void *data)
{
    memcpy(data, (void *)addr, PLAT_FLASH_PAGE_SIZE);

    return EGL_SUCCESS;
}

static egl_result_t unlock(void)
{
    #define FLASH_KEY1 (0x45670123U)
    #define FLASH_KEY2 (0xCDEF89ABU)

    /* Unlock flash memory by writing the keys to the FLASH_KEYR register */
    if(FLASH->CR & FLASH_CR_LOCK)
    {
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;

        if(FLASH->CR & FLASH_CR_LOCK)
        {
            return EGL_INVALID_STATE;
        }
    }

    return EGL_SUCCESS;
}

static egl_result_t lock(void)
{
    /* Lock the flash memory to prevent accidental writes */
    FLASH->CR |= FLASH_CR_LOCK;

    return EGL_SUCCESS;
}

static egl_result_t erase(uint32_t addr)
{
    #define FLASH_START (0x08000000)

    if(addr < FLASH_START)
    {
        return EGL_INVALID_PARAM;
    }

    uint8_t page = (addr - FLASH_START) / PLAT_FLASH_PAGE_SIZE;

    /* Set the page erase bit */
    FLASH->CR |= FLASH_CR_PER;

    /* Set the page number to erase */
    FLASH->CR &= ~FLASH_CR_PNB;
    FLASH->CR |= (page << FLASH_CR_PNB_Pos);

    /* Start the erase operation */
    FLASH->CR |= FLASH_CR_STRT;

    /* Wait for the operation to complete */
    while (FLASH->SR & FLASH_SR_BSY)
    {
        /* Wait */
    }

    /* Clear the page erase bit */
    FLASH->CR &= ~FLASH_CR_PER;

    return EGL_SUCCESS;
}

static egl_result_t programm(uint32_t addr, void *data)
{
    egl_result_t result = EGL_SUCCESS;
    uint32_t *data_ptr = data;

    /* Set the programming bit */
    FLASH->CR |= FLASH_CR_PG;

    for(uint32_t i = 0; i < PLAT_FLASH_PAGE_SIZE / 8; i++)
    {
        /* Write first word */
        *(volatile uint32_t*)addr = *data_ptr;
        addr += sizeof(uint32_t);
        data_ptr++;

        /* Write second word */
        *(volatile uint32_t*)addr = *data_ptr;
        addr += sizeof(uint32_t);
        data_ptr++;

        /* Wait for the operation to complete */
        while (FLASH->SR & FLASH_SR_BSY)
        {
            /* Wait */
        }
    }

    /* Clear the programming bit */
    FLASH->CR &= ~FLASH_CR_PG;

    return result;
}

static egl_result_t write(uint32_t addr, void *data)
{
    egl_result_t result;

    /* Disable interrupts to avoid any interruption during the loop */
    uint32_t primask_bit = __get_PRIMASK();
    __disable_irq();

    /* Wait for any previous operation to complete */
    while (FLASH->SR & FLASH_SR_BSY)
    {
        /* Wait */
    }

    result = unlock();
    if(result != EGL_SUCCESS)
    {
        goto exit;
    }

    result = erase(addr);
    if(result != EGL_SUCCESS)
    {
        goto exit;
    }

    result = programm(addr, data);
    if(result != EGL_SUCCESS)
    {
        goto exit;
    }

    result = lock();
    if(result != EGL_SUCCESS)
    {
        goto exit;
    }

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