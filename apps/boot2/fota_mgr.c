#include "egl_lib.h"
#include "plat.h"
#include "protocol.h"

#define FOTA_UPLOAD_TIMEOUT (60000U)

typedef enum
{
    FOTA_STATE_UPLOAD_INIT,
    FOTA_STATE_DOWNLOAD_INIT,
    FOTA_STATE_UPLOAD,
    FOTS_STATE_DOWNLOAD,
    FOTA_STATE_FAIL,
    FOTA_STATE_END
}fota_state_t;

static const char *fota_mgr_state_str_get(fota_state_t state)
{
    static const char *fota_state_str[] =
    {
        "FOTA_STATE_UPLOAD_INIT",
        "FOTA_STATE_DOWNLOAD_INIT",
        "FOTA_STATE_UPLOAD",
        "FOTS_STATE_DOWNLOAD",
        "FOTA_STATE_FAIL",
        "FOTA_STATE_END"
    };

    EGL_ASSERT_CHECK(state <= FOTA_STATE_END, NULL);

    return fota_state_str[state];
}

static fota_state_t fota_mgr_upload_init_step(plat_boot_slot_t target_slot, fota_state_t state)
{
    PROTOCOL_PACKET_DECLARE(request, sizeof(target_slot));
    PROTOCOL_PACKET_DECLARE(response, 0);

    egl_result_t result;
    size_t len = sizeof(request_buff);
    state = FOTA_STATE_FAIL;

    result = protocol_packet_build(request, PROTOCOL_CMD_FOTA_REQUEST_TO_DOWNLOAD,
                                   &target_slot, sizeof(target_slot));
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

    for(unsigned int attepmpts = 5; attepmpts > 0; attepmpts--)
    {
        EGL_LOG_INFO("Send request to download. Attempt: %u", attepmpts);

        result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_DEBUG, "send", request_buff, len, 8);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

        result = egl_iface_write(RADIO, request, &len);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

        result = egl_iface_read(RADIO, response, &len);
        if(result == EGL_SUCCESS)
        {
            result = protocol_packet_validate(response, PROTOCOL_CMD_ACK, 0);
            if(result == EGL_SUCCESS)
            {
                state = FOTA_STATE_UPLOAD;
                break;
            }
        }

        result = egl_sys_delay(1000);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);
    }

    return state;
}

static fota_state_t fota_mgr_download_init_step(plat_boot_slot_t target_slot, fota_state_t state)
{
    PROTOCOL_PACKET_DECLARE(response, 0);

    egl_result_t result;
    size_t len = sizeof(response_buff);
    state = FOTA_STATE_FAIL;

    result = protocol_packet_build(response, PROTOCOL_CMD_ACK, NULL, 0);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_DEBUG, "send", response_buff, len, 8);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

    result = egl_iface_write(RADIO, response, &len);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

    state = FOTS_STATE_DOWNLOAD;

    return state;
}

static egl_result_t fota_mgr_slot_start_addr_get(plat_boot_slot_t target_slot, uint32_t *slot_start)
{
    egl_result_t result;

    switch(target_slot)
    {
        case PLAT_SLOT_A:
            result = egl_value_u32_get(SLOT_A_ADDR, slot_start);
            break;

        case PLAT_SLOT_B:
            result = egl_value_u32_get(SLOT_B_ADDR, slot_start);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t fota_mgr_page_send(plat_boot_slot_t target_slot, uint32_t page_num)
{
    uint32_t addr;
    egl_result_t result;
    uint32_t slot_start;

    PROTOCOL_PACKET_DECLARE(response, PLAT_FLASH_PAGE_SIZE);
    size_t packet_len = sizeof(response_buff);

    result = fota_mgr_slot_start_addr_get(target_slot, &slot_start);
    EGL_RESULT_CHECK(result);

    addr = slot_start + page_num * PLAT_FLASH_PAGE_SIZE;

    EGL_LOG_WARN("Requested page: %u, addr: %u", page_num, addr);

    /* Read page directly to packet */
    result = egl_block_read(PLAT_FLASH, addr, response->payload);
    EGL_RESULT_CHECK(result);

    /* Left payload as NULL, cause it is already writted to the packet */
    result = protocol_packet_build(response, PROTOCOL_CMD_FOTA_SEND_PAGE, NULL, PLAT_FLASH_PAGE_SIZE);
    EGL_RESULT_CHECK(result);

    result = egl_iface_write(RADIO, response, &packet_len);
    EGL_RESULT_CHECK(result);

    return result;
}

static fota_state_t fota_mgr_upload_step(plat_boot_slot_t target_slot, fota_state_t state)
{
    size_t packet_len;
    egl_result_t result;
    uint32_t requested_page;
    uint32_t start_timestamp = egl_timer_get(SYSTIMER);

    PROTOCOL_PACKET_DECLARE(request, sizeof(requested_page));

    while(true)
    {
        /* Check for timeout */
        uint32_t current_timestamp = egl_timer_get(SYSTIMER);
        if(current_timestamp - start_timestamp > FOTA_UPLOAD_TIMEOUT)
        {
            EGL_LOG_WARN("Fota upload timeout");
            break;
        }

        /* Waiting for request page command */
        packet_len = sizeof(request);
        result = egl_iface_read(RADIO, request, &packet_len);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS || result == EGL_TIMEOUT, FOTA_STATE_FAIL);

        if(result == EGL_SUCCESS)
        {
            result = protocol_packet_validate(request, PROTOCOL_CMD_FOTA_REQUEST_PAGE, sizeof(requested_page));
            if(result == EGL_SUCCESS)
            {
                result = fota_mgr_page_send(target_slot, request->payload[0]);
                EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);
            }
            else
            {
                /* May be it is complete command ? */
                result = protocol_packet_validate(request, PROTOCOL_CMD_FOTA_COMPLETE, 0);
                if(result == EGL_SUCCESS)
                {
                    EGL_LOG_INFO("FOTA complete command received");
                    break;
                }
            }
        }
    }

    return FOTA_STATE_END;
}

static fota_state_t fota_mgr_download_step(plat_boot_slot_t target_slot, fota_state_t state)
{
    egl_result_t result;
    uint32_t current_page = 0;
    uint32_t total_pages = -1;
    size_t packet_len;
    uint32_t slot_start_addr;

    PROTOCOL_PACKET_DECLARE(request, sizeof(current_page));
    PROTOCOL_PACKET_DECLARE(response, PLAT_FLASH_PAGE_SIZE);

    result = fota_mgr_slot_start_addr_get(target_slot, &slot_start_addr);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

    do
    {
        result = protocol_packet_build(request, PROTOCOL_CMD_FOTA_REQUEST_PAGE,
                                                (uint8_t *) &current_page, sizeof(current_page));
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

        packet_len = sizeof(request_buff);
        result = egl_iface_write(RADIO, request, &packet_len);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

        packet_len = sizeof(response_buff);
        result = egl_iface_read(RADIO, response, &packet_len);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS || result == EGL_TIMEOUT, FOTA_STATE_FAIL);

        if(result == EGL_SUCCESS)
        {
            result = protocol_packet_validate(response, PROTOCOL_CMD_FOTA_SEND_PAGE, PLAT_FLASH_PAGE_SIZE);
            if(result == EGL_SUCCESS)
            {
                if(current_page == 0)
                {
                    app_info_t *info = (app_info_t *)(response->payload + CONFIG_PLAT_SLOT_INFO_OFFSET);
                    EGL_ASSERT_CHECK(info->magic == CONFIG_PLAT_SLOT_INFO_MAGIC_VALUE, FOTA_STATE_FAIL);
                    total_pages = info->size / PLAT_FLASH_PAGE_SIZE + 1;
                }

                uint32_t addr = slot_start_addr + PLAT_FLASH_PAGE_SIZE * current_page;
                result = egl_block_write(PLAT_FLASH, addr, response->payload);
                EGL_RESULT_CHECK(result);

                EGL_LOG_INFO("Download page %u/%u", current_page, total_pages);

                current_page++;
            }
            else
            {
                /* May be it is complete command ? */
                result = protocol_packet_validate(request, PROTOCOL_CMD_FOTA_COMPLETE, 0);
                if(result == EGL_SUCCESS)
                {
                    EGL_LOG_INFO("FOTA complete command received");
                    break;
                }
            }
        }
    }while(current_page != total_pages);

    return FOTA_STATE_END;
}

static fota_state_t fota_mgr_end_step(plat_boot_slot_t target_slot, fota_state_t state)
{
    egl_result_t result;

    PROTOCOL_PACKET_DECLARE(request, 0);
    size_t packet_len = sizeof(request_buff);

    result = protocol_packet_build(request, PROTOCOL_CMD_FOTA_COMPLETE, NULL, 0);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

    result = egl_iface_write(RADIO, request, &packet_len);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, FOTA_STATE_FAIL);

    return FOTA_STATE_END;
}

static fota_state_t fota_mgr_statemachine_step(plat_boot_slot_t target_slot, fota_state_t state)
{
    switch(state)
    {
        case FOTA_STATE_UPLOAD_INIT:
            state = fota_mgr_upload_init_step(target_slot, state);
            break;

        case FOTA_STATE_DOWNLOAD_INIT:
            state = fota_mgr_download_init_step(target_slot, state);
            break;

        case FOTA_STATE_UPLOAD:
            state = fota_mgr_upload_step(target_slot, state);
            break;

        case FOTS_STATE_DOWNLOAD:
            state = fota_mgr_download_step(target_slot, state);
            break;

        case FOTA_STATE_END:
            state = fota_mgr_end_step(target_slot, state);
            break;

        case FOTA_STATE_FAIL:
            state = FOTA_STATE_END;
            break;

        default:
            EGL_LOG_WARN("Wrong fota state (%u)", state);
            state = FOTA_STATE_END;
    }

    return state;
}

static egl_result_t fota_mgr_statemachine_run(plat_boot_slot_t target_slot, fota_state_t initial_state)
{
    EGL_LOG_INFO("FOTA process started. Slot: %u", target_slot);

    fota_state_t curr_state = initial_state;
    fota_state_t prev_state = FOTA_STATE_END;

    EGL_LOG_INFO("%s -> %s", fota_mgr_state_str_get(prev_state),
                             fota_mgr_state_str_get(curr_state));

    while(curr_state != FOTA_STATE_END)
    {
        curr_state = fota_mgr_statemachine_step(target_slot, curr_state);

        if(prev_state != curr_state)
        {
            EGL_LOG_INFO("%s -> %s", fota_mgr_state_str_get(prev_state),
                                     fota_mgr_state_str_get(curr_state));
            prev_state = curr_state;
        }
    }

    return EGL_SUCCESS;
}

static egl_result_t fota_mgr_iface_init(void)
{
    egl_result_t result;

    result = egl_iface_init(RADIO);
    EGL_RESULT_CHECK(result);

    result = egl_iface_ioctl(RADIO, RADIO_IOCTL_RX_TIMEOUT_SET, (void *)1000, NULL);
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t fota_mgr_process(void)
{
    egl_result_t result;
    plat_boot_config_t boot_config;

    result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT_CONFIG_GET, &boot_config, NULL);
    EGL_RESULT_CHECK(result);

    result = egl_crc_reset(PLAT_CRC);
    EGL_RESULT_CHECK(result);

    uint32_t calculated = egl_crc32_calc(PLAT_CRC, &boot_config, sizeof(plat_boot_config_t) - sizeof(uint32_t));
    EGL_ASSERT_CHECK(calculated == boot_config.checksum, EGL_CHECKSUM_MISMATCH);

    result = fota_mgr_iface_init();
    EGL_RESULT_CHECK(result);

    switch(boot_config.task)
    {
        case PLAT_FOTA_TASK_NONE:
            EGL_LOG_INFO("No boot task assigned");
            result = EGL_SUCCESS;
            break;

        case PLAT_FOTA_TASK_UPLOAD_SLOT_A:
            result = fota_mgr_statemachine_run(PLAT_SLOT_A, FOTA_STATE_UPLOAD_INIT);
            break;

        case PLAT_FOTA_TASK_UPLOAD_SLOT_B:
            result = fota_mgr_statemachine_run(PLAT_SLOT_B, FOTA_STATE_UPLOAD_INIT);
            break;

        case PLAT_FOTA_TASK_DOWNLOAD_SLOT_A:
            result = fota_mgr_statemachine_run(PLAT_SLOT_A, FOTA_STATE_DOWNLOAD_INIT);
            break;

        case PLAT_FOTA_TASK_DOWNLOAD_SLOT_B:
            result = fota_mgr_statemachine_run(PLAT_SLOT_B, FOTA_STATE_DOWNLOAD_INIT);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }
    EGL_RESULT_CHECK(result);

    return result;
}