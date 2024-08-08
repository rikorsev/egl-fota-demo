#include <string.h>

#include "egl_lib.h"
#include "plat.h"
#include "radio.h"

#define EGL_MODULE_NAME "fota"
#define FOTA_PING_MESSAGE "ping"

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

egl_result_t fota_init(void)
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
    radio->SetTxPacket(FOTA_PING_MESSAGE, sizeof(FOTA_PING_MESSAGE));
}

static fota_state_t fota_ping_process(fota_state_t state)
{
    uint8_t packet[255];
    uint16_t size = sizeof(packet);

    radio->GetRxPacket(packet, &size);

    if(size > 0)
    {
        if(strcmp((char *)packet, FOTA_PING_MESSAGE) == 0)
        {
            EGL_TRACE_INFO("Ping message received");
            state = FOTA_STATE_WAIT_FOR_ACTIONS;
        }
    }

    return state;
}

static fota_state_t fota_state_ping_step(fota_state_t state)
{
    #define FOTA_PING_TIMEOUT_MS (5000U)

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

    radio->GetRxPacket(packet, &size);

    if(packet[0] == PLAT_SLOT_A)
    {
        state = FOTA_STATE_SEND_FIRMWARE_SLOT_A;
    }
    else if(packet[0] == PLAT_SLOT_B)
    {
        state = FOTA_STATE_SEND_FIRMWARE_SLOT_A;
    }

    return state;
}

static fota_state_t fota_state_wait_for_actions_step(fota_state_t state)
{
    #define FOTA_ACTION_TIMEOUT_MS (240000U) /* 5 minutes */

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

static void print_info(egl_plat_info_t *info)
{
    if(info == NULL)
    {
        EGL_TRACE_WARN(" - No platform info found");

        return;
    }

    EGL_TRACE_INFO(" - Application: %s (v%d.%d.%d%s)", info->name,
                                                    info->version.major,
                                                    info->version.minor,
                                                    info->version.revision,
                                                    info->version.sufix);
    EGL_TRACE_INFO(" - Buildtime:   %s", info->buildtime);
    EGL_TRACE_INFO(" - Branch:      %s", info->git.branch);
    EGL_TRACE_INFO(" - Commit:      %s", info->git.commit);
    EGL_TRACE_INFO(" - Size:        %u", info->size);
    EGL_TRACE_INFO(" - Checksum:    %u (0x%08x)", info->checksum, info->checksum);
}

static fota_state_t fota_firmware_get_process(fota_state_t state)
{
    uint8_t packet[255];
    uint16_t size = sizeof(packet);

    radio->GetRxPacket(packet, &size);

    EGL_TRACE_INFO("Packet size: %u", size);

    if(size >= sizeof(egl_plat_info_t))
    {
        print_info((egl_plat_info_t *)packet);

        state = FOTA_STATE_END;
    }

    return state;
}

static fota_state_t fota_state_get_firmware_step(fota_state_t state)
{
    #define FOTA_UPDATE_TIMEOUT_MS (600000U) /* 10 minutes */

    uint8_t slot;
    uint32_t timeout = egl_timer_get(SYSTIMER) + FOTA_UPDATE_TIMEOUT_MS;

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

    /* Send request to get firmware */
    radio->SetTxPacket(&slot, sizeof(slot));

    while(egl_timer_get(SYSTIMER) < timeout &&
         (state == FOTA_STATE_GET_FIRMWARE_SLOT_A ||
          state == FOTA_STATE_GET_FIRMWARE_SLOT_B))
    {
        uint32_t curr_state = radio->Process();
        switch(curr_state)
        {
            case RF_RX_TIMEOUT:
                radio->StartRx();
                break;

            case RF_RX_DONE:
                blink(PLAT_RFM_RX_LED);
                state = fota_firmware_get_process(state);
                break;

            case RF_TX_DONE:
                blink(PLAT_RFM_TX_LED);
                radio->StartRx();

            default:
                break;
        }
    }

    return state;
}

static fota_state_t fota_state_send_firmware_step(fota_state_t state)
{
    unsigned int slot;

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
        return FOTA_STATE_END;
    }

    egl_plat_info_t *slot_info = egl_plat_slot_info(PLATFORM, slot);

    /* Send slot info packet */
    radio->SetTxPacket(slot_info, sizeof(egl_plat_info_t));

    while(state == FOTA_STATE_SEND_FIRMWARE_SLOT_A ||
          state == FOTA_STATE_SEND_FIRMWARE_SLOT_B)
    {
        uint32_t curr_state = radio->Process();
        switch(curr_state)
        {
            case RF_TX_DONE:
                blink(PLAT_RFM_TX_LED);
                radio->StartRx();
                state = FOTA_STATE_END;
                EGL_TRACE_INFO("Data sent");

            default:
                break;
        }
    }

    return state;
}

void fota_manager(void)
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

            default:
                EGL_TRACE_WARN("Wrong state %d", curr_state);
                curr_state = FOTA_STATE_END;
        }

        EGL_TRACE_INFO("%s -> %s", state_srt[prev_state], state_srt[curr_state]);

    }while(curr_state != FOTA_STATE_END);
}