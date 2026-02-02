#include "egl_lib.h"
#include "plat.h"

#define BAUDRATE_115200 (0x12U)
#define SYSLOG_THREAD_PRIORITY (16U)
#define SYSLOG_MSG_CHUNK_SIZE (32U)

extern egl_timer_t plat_systimer_inst;
static char syslog_buff[CONFIG_PLAT_SYSLOG_BUFF_SIZE];

#if CONFIG_EGL_OS_ENABLED
static void *syslog_thread = NULL;
static void *syslog_queue = NULL;
static void *syslog_mux = NULL;
#endif

static void init_tx_pin(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Configure the GPIO as an alternative function */
    GPIOA->MODER &= ~GPIO_MODER_MODE2;
    GPIOA->MODER |= GPIO_MODER_MODE2_1;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_2;

    /* Configure Low Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED2;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;

    /* Set alternative function number */
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
    GPIOA->AFR[0] |= 0x4 << GPIO_AFRL_AFSEL2_Pos;
}

static void init_uart(void)
{
    /* Enable UART clock */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Set baud rate */
    USART2->BRR = BAUDRATE_115200;

    /* Enable transmitter */
    USART2->CR1 |= USART_CR1_TE;

    /* Enable UART */
    USART2->CR1 |= USART_CR1_UE;
}

/* PA2 PA3 */
static egl_result_t init(void)
{
    init_tx_pin();
    init_uart();

    return EGL_SUCCESS;
}

static egl_result_t write_uart(void *data, size_t *len)
{
    for(uint32_t i = 0; i < *len; i++)
    {
        while(!(USART2->ISR & USART_ISR_TXE))
        {
            /* Wait for character transmission */
        }
        USART2->TDR = ((uint8_t *)data)[i];
    }

    return EGL_SUCCESS;
}

#if CONFIG_EGL_OS_ENABLED
static egl_result_t write_queue(void *data, size_t *len)
{
    egl_result_t result;

    result = egl_os_mux_lock(SYSOS, syslog_mux, EGL_OS_WAIT_FOREWER);
    EGL_RESULT_CHECK(result);

    for(unsigned int i = 0; i < *len; i += SYSLOG_MSG_CHUNK_SIZE)
    {
        uint8_t chunk[SYSLOG_MSG_CHUNK_SIZE] = {0};
        size_t size = i + SYSLOG_MSG_CHUNK_SIZE > *len ? *len - i : SYSLOG_MSG_CHUNK_SIZE;

        memcpy(chunk, &((uint8_t *)data)[i], size);

        result = egl_os_queue_put(SYSOS, syslog_queue, chunk, EGL_OS_WAIT_FOREWER);
        EGL_RESULT_CHECK_EXIT(result);
    }
    
exit:
    result = egl_os_mux_unlock(SYSOS, syslog_mux);
    EGL_RESULT_CHECK(result);

    return result;
}

static void plat_syslog_thread_entry(void *data)
{
    while(1)
    {
        uint8_t chunk[SYSLOG_MSG_CHUNK_SIZE] = {0};
        size_t size = sizeof(chunk);
        egl_os_queue_get(SYSOS, syslog_queue, &chunk, EGL_OS_WAIT_FOREWER);
        write_uart(chunk, &size);
    }
}

static egl_result_t init_thread(void)
{
    egl_result_t result;
    static egl_os_thread_ctx thread_ctx;
    static egl_os_queue_ctx queue_ctx;
    static egl_os_mux_ctx mux_ctx;
    static uint8_t stack[1024];
    static uint8_t queue[CONFIG_PLAT_SYSLOG_BUFF_SIZE];

    result = egl_os_mux_create(SYSOS, &syslog_mux, "syslog_mux",
                               EGL_OS_MUX_TYPE_REGULAR, &mux_ctx);
    EGL_RESULT_CHECK(result);

    result = egl_os_queue_create(SYSOS, &syslog_queue, "syslog_queue",
                                 CONFIG_PLAT_SYSLOG_BUFF_SIZE, SYSLOG_MSG_CHUNK_SIZE,
                                 queue, sizeof(queue), &queue_ctx);
    EGL_RESULT_CHECK(result);

    result = egl_os_thread_create(SYSOS, &syslog_thread, "syslog",
                                  plat_syslog_thread_entry,
                                  NULL, stack, sizeof(stack),
                                  SYSLOG_THREAD_PRIORITY, &thread_ctx);
    EGL_RESULT_CHECK(result);

    return result;
}
#endif

static egl_result_t write(void *data, size_t *len)
{
    egl_result_t result;
#if CONFIG_EGL_OS_ENABLED
    if(__builtin_expect(syslog_thread == NULL, 0))
    {
        egl_os_state_t state;
        result = egl_os_state_get(SYSOS, &state);
        EGL_RESULT_CHECK(result);

        if(state == EGL_OS_RUNNING)
        {
            result = init_thread();
            EGL_RESULT_CHECK(result);
        }
        else
        {
#endif
            result = write_uart(data, len);
            EGL_RESULT_CHECK(result);
#if CONFIG_EGL_OS_ENABLED
        }
    }
    else
    {
        result = write_queue(data, len);
        EGL_RESULT_CHECK(result);
    }
#endif

    return result;
}

static const egl_iface_t plat_syslog_iface =
{
    .init = init,
    .write = write
};

egl_log_t plat_syslog_inst =
{
    .iface = (egl_iface_t *)&plat_syslog_iface,
#if CONFIG_EGL_OS_ENABLED
    .frontend = egl_log_frontend_default_os,
#else
    .frontend = egl_log_frontend_default_bare,
#endif
    .timer = &plat_systimer_inst,
    .buff = syslog_buff,
    .size = sizeof(syslog_buff)
};

egl_log_t *plat_syslog_get(void)
{
    return &plat_syslog_inst;
}

