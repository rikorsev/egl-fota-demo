#include "egl_lib.h"
#include "plat.h"
#include "protocol.h"
#include "fota_mgr.h"

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
        case FOTA_STATE_FAIL:
            state = fota_mgr_end_step(target_slot, state);
            break;

        default:
            EGL_LOG_WARN("Wrong fota state (%u)", state);
            state = FOTA_STATE_END;
    }

    return state;
}

egl_result_t fota_mgr_process(plat_boot_slot_t target_slot, fota_task_t task)
{
    fota_state_t curr_state;

    switch(task)
    {
        case FOTA_MGR_TASK_DOWNLOAD:
            curr_state = FOTA_STATE_DOWNLOAD_INIT;
            break;

        case FOTA_MGR_TASK_UPLOAD:
            curr_state = FOTA_STATE_UPLOAD_INIT;
            break;

        default:
            EGL_LOG_WARN("Unknown task %u", task);
            curr_state = FOTA_STATE_FAIL;
    }

    fota_state_t prev_state = FOTA_STATE_END;

    EGL_LOG_INFO("%s -> %s", fota_mgr_state_str_get(prev_state),
                             fota_mgr_state_str_get(curr_state));

    while(curr_state != FOTA_STATE_END && curr_state != FOTA_STATE_FAIL)
    {
        curr_state = fota_mgr_statemachine_step(target_slot, curr_state);

        if(prev_state != curr_state)
        {
            EGL_LOG_INFO("%s -> %s", fota_mgr_state_str_get(prev_state),
                                     fota_mgr_state_str_get(curr_state));
            prev_state = curr_state;
        }
    }

    return curr_state == FOTA_STATE_END ? EGL_SUCCESS : EGL_FAIL;
}


static egl_result_t event_fota_check_handle(void)
{
    egl_result_t result;
    size_t packet_len;

    EGL_LOG_INFO("Check for new FOTA available");

    PROTOCOL_PACKET_DECLARE(request, 0);
    PROTOCOL_PACKET_DECLARE(response, sizeof(uint8_t));

    result = protocol_packet_build(request, PROTOCOL_CMD_FOTA_STATUS_REQUEST, NULL, 0);
    EGL_RESULT_CHECK(result);

    packet_len = sizeof(request_buff);
    result = egl_iface_write(RADIO, request, &packet_len);
    EGL_RESULT_CHECK(result);

    packet_len = sizeof(response_buff);
    result = egl_iface_read(RADIO, response, &packet_len);
    EGL_RESULT_CHECK(result);

    result = protocol_packet_validate(response, PROTOCOL_CMD_FOTA_STATUS_RESPONSE, sizeof(uint8_t));
    EGL_RESULT_CHECK(result);

    is_fota_available = response->payload[0];

    EGL_LOG_INFO("FOTA status: %d", is_fota_available);

    return result;
}