#include <string.h>

#include "egl_lib.h"
#include "plat.h"
#include "radio.h"
#include "sx1232.h"
#include "sx1232-Hal.h"
#include "sx1232-Misc.h"
#include "boot_mgr.h"

#define EGL_MODULE_NAME "fota"

#define FOTA_DATA_PACKET_PAYLOAD      (128U)
#define FOTA_MARKER_PING              (0x50494E47)
#define FOTA_MARKER_SLOT              (0x534C4F54)
#define FOTA_MARKER_DATA              (0x44415441)
#define FOTA_MARKER_ACK               (0x41434b20)
#define FOTA_MARKER_REQ               (0x52455120)
#define FOTA_MARKER_END               (0x454E4420)
#define FOTA_UPDATE_TIMEOUT_MS        (600000U)    /* 10 minutes */
#define FOTA_END_TIMEOUT_MS           (5000U)      /* 5 seconds */
#define FOTA_ACTION_TIMEOUT_MS        (240000U)    /* 5 minutes */
#define FOTA_PING_TIMEOUT_MS          (5000U)      /* 5 seconds */
#define FOTA_RX_TIMEOUT_COUNT_LIMIT   (100u)

typedef enum
{
    FOTA_PACKET_STATUS_OK,
    FOTA_PACKET_WRONG_MARKER,
    FOTA_PACKET_WRONG_SIZE,
    FOTA_PACKET_CRC_ERROR,
    FOTA_PACKET_END_MARKER,
}fota_packet_status_t;

typedef enum
{
    FOTA_STATE_START,
    FOTA_STATE_PING,
    FOTA_STATE_WAIT_FOR_ACTIONS,
    FOTA_STATE_GET_FIRMWARE_SLOT_A,
    FOTA_STATE_GET_FIRMWARE_SLOT_B,
    FOTA_STATE_SEND_FIRMWARE_SLOT_A,
    FOTA_STATE_SEND_FIRMWARE_SLOT_B,
    FOTA_STATE_END
}fota_state_t;

typedef struct
{
    uint32_t marker;
    uint32_t page;
    uint32_t offset;
    uint8_t payload[FOTA_DATA_PACKET_PAYLOAD];
    uint32_t crc;
}fota_data_packet_t;

typedef struct
{
    uint32_t marker;
    uint32_t page;
    uint32_t offset;
}fota_req_packet_t;

typedef struct
{
    uint32_t marker;
    uint32_t slot;
}fota_slot_packet_t;

static tRadioDriver *radio;
static bool update_slot_a;
static bool update_slot_b;

static void fota_sw1_callback(void *data)
{
    update_slot_b = true;
}

static void fota_sw2_callback(void *data)
{
    update_slot_a = true;
}

egl_result_t fota_mgr_init(void)
{
    egl_result_t result;

    radio = RadioDriverInit();

    if(radio == NULL)
    {
        return EGL_FAIL;
    }

    radio->Init();
    radio->StartRx();

    result = egl_pio_init(PLAT_RFM_TX_LED);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm tx led pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_init(PLAT_RFM_RX_LED);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm rx led pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_init(PLAT_RFM_SW1);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init sw1 pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_callback_set(PLAT_RFM_SW1, fota_sw1_callback);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to set callback for sw1 pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_init(PLAT_RFM_SW2);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init sw2 pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_callback_set(PLAT_RFM_SW2, fota_sw2_callback);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to set callback for sw2 pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    /* WORKAROUND: It seems that SX1232 doesn't like sequential transmission of
       0xFFFFFFFF or 0x00000000 bytes. So that, RF module has posibility to encode decode dacket
       data in Manchester format. Here we enable such encoding */
    uint8_t reg;
    SX1232Read(REG_PACKETCONFIG1, &reg);
    reg &= RF_PACKETCONFIG1_DCFREE_MASK;
    reg |= RF_PACKETCONFIG1_DCFREE_MANCHESTER;
    SX1232Write(REG_PACKETCONFIG1, reg);

    /* Set bitrate to 115200 */
    SX1232SetBitrate(115200);

    return EGL_SUCCESS;
}

static void blink(egl_pio_t *pio)
{
    egl_pio_set(pio, true);
    egl_plat_sleep(PLATFORM, 10);
    egl_pio_set(pio, false);
}

static void fota_ping_send(void)
{
    uint32_t ping = FOTA_MARKER_PING;

    radio->SetTxPacket(&ping, sizeof(ping));
}

static fota_state_t fota_ping_process(fota_state_t state)
{
    uint8_t packet[255];
    uint16_t size = sizeof(packet);

    radio->GetRxPacket(packet, &size);

    if(size == sizeof(uint32_t))
    {
        if(((uint32_t *)packet)[0] == FOTA_MARKER_PING)
        {
            EGL_TRACE_INFO("Ping message received");
            state = FOTA_STATE_WAIT_FOR_ACTIONS;
        }
    }

    return state;
}

static fota_state_t fota_state_ping_step(fota_state_t state)
{
    uint32_t timeout = egl_timer_get(SYSTIMER) + FOTA_PING_TIMEOUT_MS;

    while(egl_timer_get(SYSTIMER) < timeout)
    {
        uint32_t curr_state = radio->Process();
        switch(curr_state)
        {
            case RF_RX_TIMEOUT:
                fota_ping_send();
                break;

            case RF_RX_DONE:
                blink(PLAT_RFM_RX_LED);
                state = fota_ping_process(state);
                if(state == FOTA_STATE_WAIT_FOR_ACTIONS)
                {
                    fota_ping_send();
                }
                break;

            case RF_TX_DONE:
                blink(PLAT_RFM_TX_LED);
                if(state == FOTA_STATE_PING)
                {
                    radio->StartRx();
                }
                else
                {
                    fota_ping_send();
                }
                break;

            default:
                break;
        }
    }

    if(state == FOTA_STATE_PING)
    {
        state = FOTA_STATE_END;
    }

    return state;
}

static fota_state_t fota_request_to_update_process(void)
{
    uint8_t packet[255];
    uint16_t size = sizeof(packet);
    fota_state_t state = FOTA_STATE_WAIT_FOR_ACTIONS;
    fota_slot_packet_t *packet_ptr = (fota_slot_packet_t *)packet;

    radio->GetRxPacket(packet, &size);

    if(size == sizeof(fota_slot_packet_t) && packet_ptr->marker == FOTA_MARKER_SLOT)
    {
        if(packet_ptr->slot == PLAT_SLOT_A)
        {
            state = FOTA_STATE_SEND_FIRMWARE_SLOT_A;
        }
        else if(packet_ptr->slot == PLAT_SLOT_B)
        {
            state = FOTA_STATE_SEND_FIRMWARE_SLOT_B;
        }
    }

    return state;
}

static fota_state_t fota_state_wait_for_actions_step(fota_state_t state)
{
    uint32_t timeout = egl_timer_get(SYSTIMER) + FOTA_ACTION_TIMEOUT_MS;

    EGL_TRACE_INFO("Ready for FOTA");
    EGL_TRACE_INFO("Press SW1 to update slot A");
    EGL_TRACE_INFO("Press SW2 to update slot B");

    radio->StartRx();

    while(egl_timer_get(SYSTIMER) < timeout && state == FOTA_STATE_WAIT_FOR_ACTIONS)
    {
        uint32_t curr_state = radio->Process();
        switch(curr_state)
        {
            case RF_RX_TIMEOUT:
                radio->StartRx();
                break;

            case RF_RX_DONE:
                blink(PLAT_RFM_RX_LED);
                state = fota_request_to_update_process();
                break;

            default:
                break;
        }

        if(update_slot_a)
        {
            state = FOTA_STATE_GET_FIRMWARE_SLOT_A;
        }
        else if(update_slot_b)
        {
            state = FOTA_STATE_GET_FIRMWARE_SLOT_B;
        }
    }

    /* Timeout */
    if(state == FOTA_STATE_WAIT_FOR_ACTIONS)
    {
        state = FOTA_STATE_END;
    }

    return state;
}

void print_block(uint32_t addr, uint8_t *buffer, size_t size)
{
    for(int i = 0; i < (size / sizeof(uint32_t)); i+=8)
    {
        EGL_TRACE_INFO("%08x: %08x %08x %08x %08x %08x %08x %08x %08x", addr + i * sizeof(uint32_t),
                                                                        ((uint32_t *)buffer)[i],
                                                                        ((uint32_t *)buffer)[i + 1],
                                                                        ((uint32_t *)buffer)[i + 2],
                                                                        ((uint32_t *)buffer)[i + 3],
                                                                        ((uint32_t *)buffer)[i + 4],
                                                                        ((uint32_t *)buffer)[i + 5],
                                                                        ((uint32_t *)buffer)[i + 6],
                                                                        ((uint32_t *)buffer)[i + 7]);
    }
}

static void fota_req_packet_send(uint32_t page, uint32_t offset)
{
    fota_req_packet_t packet =
    {
        .marker = FOTA_MARKER_REQ,
        .page = page,
        .offset = offset
    };

    EGL_TRACE_INFO("Requ: page %u, offset %u", page, offset);

    radio->SetTxPacket(&packet, sizeof(packet));
}

static fota_packet_status_t fota_data_packet_get(fota_data_packet_t *data)
{
    uint8_t packet[255];
    uint16_t packet_size;
    fota_data_packet_t *packet_ptr = (fota_data_packet_t *)packet;

    radio->GetRxPacket(packet, &packet_size);

    if(((uint32_t *)packet)[0] == FOTA_MARKER_END)
    {
        EGL_TRACE_INFO("End marker received");
        return FOTA_PACKET_END_MARKER;
    }

    if(packet_size != sizeof(fota_data_packet_t))
    {
        EGL_TRACE_ERROR("Wrong packet size. Expected %u, actual %u",
                                                           sizeof(fota_data_packet_t), packet_size);
        return FOTA_PACKET_WRONG_SIZE;
    }

    if(packet_ptr->marker != FOTA_MARKER_DATA)
    {
        EGL_TRACE_ERROR("Wrong packet marker");
        return FOTA_PACKET_WRONG_MARKER;
    }

    egl_result_t result = egl_crc_reset(PLAT_CRC);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_FAIL("Fail to reset CRC. Result: %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    uint32_t calc_crc = egl_crc32_calc(PLAT_CRC, packet_ptr->payload, sizeof(packet_ptr->payload));
    if(calc_crc != packet_ptr->crc)
    {
        EGL_TRACE_ERROR("Packet CRC missmatch");
        EGL_TRACE_ERROR("Expected: 0x%08x, Calculated: 0x%08x", packet_ptr->crc, calc_crc);

        return FOTA_PACKET_CRC_ERROR;
    }

    memcpy(data, packet, sizeof(fota_data_packet_t));

    return FOTA_PACKET_STATUS_OK;
}

static fota_state_t fota_state_get_firmware_step(fota_state_t state)
{
    uint8_t slot;
    uint32_t timeout = egl_timer_get(SYSTIMER) + FOTA_UPDATE_TIMEOUT_MS;
    uint8_t page_buff[PLAT_FLASH_PAGE_SIZE];
    uint32_t curr_page = 0;
    uint32_t curr_offset = 0;
    fota_data_packet_t data;
    egl_plat_info_t *slot_info = NULL;
    uint32_t timeout_cnt = 0;

    if(state == FOTA_STATE_GET_FIRMWARE_SLOT_A)
    {
        slot = PLAT_SLOT_A;
    }
    else if(state == FOTA_STATE_GET_FIRMWARE_SLOT_B)
    {
        slot = PLAT_SLOT_B;
    }
    else
    {
        return FOTA_STATE_END;
    }

    /* Get initial address to flash */
    uint32_t slot_start_addr = plat_info_slot_addr_get(slot);
    if(slot_start_addr == 0)
    {
        EGL_TRACE_ERROR("Fail to get slot address");
        return FOTA_STATE_END;
    }

    /* Send reqiest to start firmware update */
    fota_slot_packet_t slot_packet = { .marker = FOTA_MARKER_SLOT, .slot = slot };
    radio->SetTxPacket(&slot_packet, sizeof(slot_packet));

    while(egl_timer_get(SYSTIMER) < timeout)
    {
        uint32_t curr_state = radio->Process();
        switch(curr_state)
        {
            case RF_TX_DONE:
                blink(PLAT_RFM_TX_LED);
                radio->StartRx();
                break;

            case RF_TX_TIMEOUT:
                EGL_TRACE_WARN("TX timeout");
                fota_req_packet_send(curr_page, curr_offset);
                break;

            case RF_RX_TIMEOUT:
                EGL_TRACE_WARN("RX timeout");
                if(++timeout_cnt > FOTA_RX_TIMEOUT_COUNT_LIMIT)
                {
                    EGL_TRACE_ERROR("RX timeout limit reached");
                    return FOTA_STATE_END;
                }

                fota_req_packet_send(curr_page, curr_offset);
                break;

            case RF_RX_DONE:
                blink(PLAT_RFM_RX_LED);
                timeout_cnt = 0;
                fota_packet_status_t packet_status = fota_data_packet_get(&data);
                if(packet_status != FOTA_PACKET_STATUS_OK)
                {
                    if(packet_status == FOTA_PACKET_END_MARKER)
                    {
                        return FOTA_STATE_END;
                    }

                    fota_req_packet_send(curr_page, curr_offset);
                    break;
                }

                if(data.page != curr_page || data.offset != curr_offset)
                {
                    EGL_TRACE_WARN("Requested/received data missmatch");
                    EGL_TRACE_WARN("Page: req %u, recv: %u", curr_page, data.page);
                    EGL_TRACE_WARN("OFFSET: req %u, recv: %u", curr_offset, data.offset);

                    fota_req_packet_send(curr_page, curr_offset);
                    break;
                }

                EGL_TRACE_INFO("Recv: page %u, offset %u", curr_page, curr_offset);

                memcpy(page_buff + curr_offset, data.payload, sizeof(data.payload));

                curr_offset += sizeof(data.payload);

                if(curr_offset == PLAT_FLASH_PAGE_SIZE)
                {
                    curr_offset = 0;

                    uint32_t addr = slot_start_addr + curr_page * PLAT_FLASH_PAGE_SIZE;

                    /* Increment update number */
                    if(curr_page == 0)
                    {
                        slot_info = (egl_plat_info_t *)page_buff;

                        slot_info->boot_number = boot_mgr_highest_boot_number_get() + 1;
                    }

                    /* Write page on flash */
                    egl_result_t result = egl_block_write(PLAT_FLASH, addr, page_buff);
                    if(result != EGL_SUCCESS)
                    {
                        EGL_TRACE_ERROR("Fail to read flash page. Result: %s", EGL_RESULT(result));
                        return FOTA_STATE_END;
                    }

                    /* If it is first page has been writted,
                       now we may read slot info and get how many pages shall be requested more */
                    if(curr_page == 0)
                    {
                        slot_info = egl_plat_slot_info(PLATFORM, slot);
                        if(slot_info == NULL)
                        {
                            EGL_TRACE_ERROR("Firmware validation fail");
                            return FOTA_STATE_END;
                        }
                    }

                    curr_page++;
                    if(curr_page >= slot_info->size / PLAT_FLASH_PAGE_SIZE + 1)
                    {
                        EGL_TRACE_INFO("Firmware update complete");

                        result = boot_mgr_init();
                        if(result != EGL_SUCCESS)
                        {
                            EGL_TRACE_WARN("Fail to reinit boot manager. Result: %s",
                                                                                EGL_RESULT(result));
                        }

                        return FOTA_STATE_END;
                    }
                }
                else if(curr_offset > PLAT_FLASH_PAGE_SIZE)
                {
                    EGL_TRACE_ERROR("Buffer overflow");
                    return FOTA_STATE_END;
                }

                fota_req_packet_send(curr_page, curr_offset);
                break;

            default:
                break;
        }
    }

    return FOTA_STATE_END;
}

static fota_packet_status_t fota_request_packet_get(fota_req_packet_t *req)
{
    uint8_t packet[255];
    uint16_t packet_size;

    radio->GetRxPacket(packet, &packet_size);

    if(((uint32_t *)packet)[0] == FOTA_MARKER_END)
    {
        EGL_TRACE_INFO("Got end marker");
        return FOTA_PACKET_END_MARKER;
    }

    if(packet_size != sizeof(fota_req_packet_t))
    {
        EGL_TRACE_ERROR("Wrong packet size");
        return FOTA_PACKET_WRONG_SIZE;
    }

    if(((fota_req_packet_t *)packet)->marker != FOTA_MARKER_REQ)
    {
        EGL_TRACE_ERROR("Wrong marker");
        return FOTA_PACKET_WRONG_MARKER;
    }

    memcpy(req, packet, sizeof(fota_req_packet_t));

    return FOTA_PACKET_STATUS_OK;
}

static void fota_data_packet_send(uint8_t *buff, uint32_t page, uint32_t offset)
{
    fota_data_packet_t packet =
    {
        .marker = FOTA_MARKER_DATA,
        .page = page,
        .offset = offset,
    };

    /* Copy payload */
    memcpy(packet.payload, buff, sizeof(packet.payload));

    /* Calculate payload CRC */
    egl_result_t result = egl_crc_reset(PLAT_CRC);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_FAIL("Fail to reset CRC. Result: %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    packet.crc = egl_crc32_calc(PLAT_CRC, packet.payload, sizeof(packet.payload));

    radio->SetTxPacket(&packet, sizeof(packet));
}

static fota_state_t fota_state_send_firmware_step(fota_state_t state)
{
    unsigned int slot;
    fota_req_packet_t req;
    uint8_t page_buff[PLAT_FLASH_PAGE_SIZE];
    uint32_t curr_page_num = 0xFFFFFFFF;
    uint32_t timeout = egl_timer_get(SYSTIMER) + FOTA_UPDATE_TIMEOUT_MS;
    egl_result_t result;

    if(state == FOTA_STATE_SEND_FIRMWARE_SLOT_A)
    {
        slot = PLAT_SLOT_A;
    }
    else if(state == FOTA_STATE_SEND_FIRMWARE_SLOT_B)
    {
        slot = PLAT_SLOT_B;
    }
    else
    {
        EGL_TRACE_ERROR("Incorrect slot");
        return FOTA_STATE_END;
    }

    egl_plat_info_t *slot_info = egl_plat_slot_info(PLATFORM, slot);
    if(slot_info == NULL)
    {
        EGL_TRACE_ERROR("No slot info detected");
        return FOTA_STATE_END;
    }

    result = boot_mgr_slot_validate(slot_info);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Binary validation fail. Result: %s", EGL_RESULT(result));
        return FOTA_STATE_END;
    }

    uint32_t slot_start_addr = (uint32_t)slot_info;

    while(egl_timer_get(SYSTIMER) < timeout)
    {
        uint32_t curr_state = radio->Process();
        switch(curr_state)
        {
            case RF_TX_DONE:
                blink(PLAT_RFM_TX_LED);
                EGL_TRACE_INFO("Sent: page %u, offset %u", req.page, req.offset);
                /* No break here */

            case RF_TX_TIMEOUT:
            case RF_RX_TIMEOUT:
                radio->StartRx();
                break;

            case RF_RX_DONE:
                blink(PLAT_RFM_RX_LED);
                fota_packet_status_t packet_status = fota_request_packet_get(&req);
                if(packet_status == FOTA_PACKET_STATUS_OK)
                {
                    EGL_TRACE_INFO("Req: page %u, offset %u", req.page, req.offset);

                    if(req.offset + FOTA_DATA_PACKET_PAYLOAD > PLAT_FLASH_PAGE_SIZE)
                    {
                        EGL_TRACE_WARN("Invalid offset");
                        radio->StartRx();
                        break;
                    }

                    if(req.page > slot_info->size / PLAT_FLASH_PAGE_SIZE +1)
                    {
                        EGL_TRACE_WARN("Invalid offset");
                        radio->StartRx();
                        break;
                    }

                    if(curr_page_num != req.page)
                    {
                        uint32_t addr = slot_start_addr + req.page * PLAT_FLASH_PAGE_SIZE;

                        result = egl_block_read(PLAT_FLASH, addr, page_buff);
                        if(result != EGL_SUCCESS)
                        {
                            EGL_TRACE_ERROR("Fail to read flash page. Result: %s",
                                                                                EGL_RESULT(result));
                            return FOTA_STATE_END;
                        }

                        curr_page_num = req.page;
                    }

                    fota_data_packet_send(page_buff + req.offset, req.page, req.offset);
                }
                else if(packet_status == FOTA_PACKET_END_MARKER)
                {
                    return FOTA_STATE_END;
                }
                else
                {
                    radio->StartRx();
                }
                break;

            default:
                break;
        }
    }

    return FOTA_STATE_END;
}

static fota_state_t fota_state_end_step(fota_state_t state)
{
    uint32_t end_marker = FOTA_MARKER_END;
    uint32_t timeout = egl_timer_get(SYSTIMER) + FOTA_END_TIMEOUT_MS;

    radio->SetTxPacket(&end_marker, sizeof(end_marker));
    while(egl_timer_get(SYSTIMER) < timeout)
    {
        uint32_t curr_state = radio->Process();
        switch(curr_state)
        {
            case RF_TX_DONE:
                blink(PLAT_RFM_TX_LED);
                return FOTA_STATE_END;

            case RF_TX_TIMEOUT:
                radio->SetTxPacket(&end_marker, sizeof(end_marker));
                break;
            
            default:
                break;
        }
    }

    return FOTA_STATE_END;
}

void fota_mgr_process(void)
{
    static const char *state_srt[] =
    {
        "FOTA_STATE_START",
        "FOTA_STATE_PING",
        "FOTA_STATE_WAIT_FOR_ACTIONS",
        "FOTA_STATE_GET_FIRMWARE_SLOT_A",
        "FOTA_STATE_GET_FIRMWARE_SLOT_B",
        "FOTA_STATE_SEND_FIRMWARE_SLOT_A",
        "FOTA_STATE_SEND_FIRMWARE_SLOT_B",
        "FOTA_STATE_END"
    };

    fota_state_t curr_state = FOTA_STATE_START;
    fota_state_t prev_state;

    do
    {
        prev_state = curr_state;

        switch(curr_state)
        {
            case FOTA_STATE_START:
                curr_state = FOTA_STATE_PING;
                break;

            case FOTA_STATE_PING:
                curr_state = fota_state_ping_step(curr_state);
                break;

            case FOTA_STATE_WAIT_FOR_ACTIONS:
                curr_state = fota_state_wait_for_actions_step(curr_state);
                break;

            case FOTA_STATE_GET_FIRMWARE_SLOT_A:
            case FOTA_STATE_GET_FIRMWARE_SLOT_B:
                curr_state = fota_state_get_firmware_step(curr_state);
                break;

            case FOTA_STATE_SEND_FIRMWARE_SLOT_A:
            case FOTA_STATE_SEND_FIRMWARE_SLOT_B:
                curr_state = fota_state_send_firmware_step(curr_state);
                break;

            case FOTA_STATE_END:
                curr_state = fota_state_end_step(curr_state);
                return;

            default:
                EGL_TRACE_WARN("Wrong state %d", curr_state);
                curr_state = FOTA_STATE_END;
        }

        EGL_TRACE_INFO("%s -> %s", state_srt[prev_state], state_srt[curr_state]);

    }while(true);
}