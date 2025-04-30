#include "egl_lib.h"
#include "plat.h"

#define EGL_MODULE_NAME "rfm_test"

static egl_result_t rfm_version_test_run(void)
{
    egl_result_t result;
    uint8_t version;

    /* Check version */
    result = egl_rfm69_read_byte(PLAT_RFM69, EGL_RFM69_REG_VERSION, &version);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to read rfm69 version. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RFM version: 0x%02x", version);

    return result;
}

static egl_result_t rfm_bitrate_test_run(void)
{
    egl_result_t result;
    uint32_t bitrate;

    result = egl_rfm69_bitrate_get(PLAT_RFM69, &bitrate);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get rfm69 bitrate. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RFM bitrate: %u", bitrate);

    result = egl_rfm69_bitrate_set(PLAT_RFM69, 115200);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set rfm69 bitrate. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static egl_result_t rfm_mode_test_run(void)
{
    egl_result_t result;
    egl_rfm69_mode_t mode;

    result = egl_rfm69_mode_get(PLAT_RFM69, &mode);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get rfm69 mode. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RFM mode: %u", mode);

    result = egl_rfm69_mode_set(PLAT_RFM69, EGL_RFM69_RX_MODE);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set rfm69 mode. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static egl_result_t rfm_data_modul_test_run(void)
{
    egl_result_t result;
    egl_rfm69_modulation_shaping_t modshap;
    egl_rfm69_modulation_type_t    modtype;
    egl_rfm69_data_mode_t          datamode;

    result = egl_rfm69_modulation_shaping_get(PLAT_RFM69, &modshap);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get modulation shaping. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_modulation_type_get(PLAT_RFM69, &modtype);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get modulation type. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_data_mode_get(PLAT_RFM69, &datamode);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get data mode. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("Modulation shaping: %u", modshap);
    EGL_LOG_INFO("Modulation type: %u", modtype);
    EGL_LOG_INFO("Data mode: %u", datamode);

    result = egl_rfm69_modulation_shaping_set(PLAT_RFM69, EGL_RFM69_MODULATION_SHAPING_1);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set modulation shaping. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_modulation_type_set(PLAT_RFM69, EGL_RFM69_MODULATION_TYPE_OOK);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set modulation type. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_data_mode_set(PLAT_RFM69, EGL_RFM69_DATA_MODE_CONTINIOUS_W_SYNC);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set data mode. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static egl_result_t rfm_deviation_test_run(void)
{
    egl_result_t result;
    uint32_t deviation;

    result = egl_rfm69_deviation_get(PLAT_RFM69, &deviation);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get deviation. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("Deviation: %u", deviation);

    result = egl_rfm69_deviation_set(PLAT_RFM69, 30000);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set deviation. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static egl_result_t rfm_frequency_test_run(void)
{
    egl_result_t result;
    uint32_t frequency;

    result = egl_rfm69_frequency_get(PLAT_RFM69, &frequency);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get frequency. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("Frequency: %u", frequency);

    result = egl_rfm69_frequency_set(PLAT_RFM69, 868000000);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set frequency. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static egl_result_t rfm_rc_calib_test_run(void)
{
    egl_result_t result;
    egl_rfm69_rc_calib_state_t state;

    result = egl_rfm69_mode_set(PLAT_RFM69, EGL_RFM69_STANDBY_MODE);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set rfm69 mode. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm_rc_calib_state_get(PLAT_RFM69, &state);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get RC calib state. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RC calib state: %u", state);

    result = egl_rfm_rc_calib_start(PLAT_RFM69);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to start RC calibration. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RC calibration start");

    do
    {
        result = egl_rfm_rc_calib_state_get(PLAT_RFM69, &state);
        if(result != EGL_SUCCESS)
        {
            EGL_LOG_ERROR("Fail to get RC calib state. Result: %s", EGL_RESULT(result));
            return result;
        }
        EGL_LOG_INFO("RC calibration in progress...");
    }while(state != EGL_RFM69_RC_CALIB_STATE_DONE);

    EGL_LOG_INFO("RC calibration done");

    return result;
}

static egl_result_t rfm_afc_ctrl_test_run(void)
{
    egl_result_t result;
    egl_rfm69_afc_routine_t routine;

    result = egl_rfm69_afc_routine_get(PLAT_RFM69, &routine);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get AFC routine type. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("AFC routine: %u", routine);

    result = egl_rfm69_afc_routine_set(PLAT_RFM69, EGL_RFM69_AFC_ROUTINE_IMPROVED);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set AFC routine type. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static egl_result_t rfm_listen_test_run(void)
{
    egl_result_t result;
    egl_rfm69_listen_end_t end_action;
    egl_rfm69_listen_criteria_t criteria;
    egl_rfm69_listen_resolution_t rx_resolution;
    egl_rfm69_listen_resolution_t idle_resolution;
    uint8_t idle_coef;
    uint8_t rx_coef;

    result = egl_rfm69_listen_end_get(PLAT_RFM69, &end_action);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get listen end. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_criteria_get(PLAT_RFM69, &criteria);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get listen criteria. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_rx_resolution_get(PLAT_RFM69, &rx_resolution);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get listen RX resolution. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_idle_resolution_get(PLAT_RFM69, &idle_resolution);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get listen IDLE resolution. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_rx_coef_get(PLAT_RFM69, &rx_coef);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get RX coefficient. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_idle_coef_get(PLAT_RFM69, &idle_coef);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get IDLE coefficient. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("Listen end action: %u", end_action);
    EGL_LOG_INFO("Listen criteria: %u", criteria);
    EGL_LOG_INFO("Listen RX resolution: %u", rx_resolution);
    EGL_LOG_INFO("Listen IDLE resolution: %u", idle_resolution);
    EGL_LOG_INFO("Listen RX coefficient: %u", rx_coef);
    EGL_LOG_INFO("Listen IDLE coefficient: %u", idle_coef);

    result = egl_rfm69_listen_end_set(PLAT_RFM69, EGL_RFM69_LISTEN_STAY_RX);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set listen end action. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_criteria_set(PLAT_RFM69, EGL_RFM69_LISTEN_CRITERIA_ABOVE_RSSI_AND_SYNC_ADDR);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set listen criteria. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_rx_resolution_set(PLAT_RFM69, EGL_RFM69_LISTEN_RESOL_4_1MS);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set listen RX resolution. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_idle_resolution_set(PLAT_RFM69, EGL_RFM69_LISTEN_RESOL_262MS);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set listen IDLE resolution. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_rx_coef_set(PLAT_RFM69, 11);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set RX coefficient. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_listen_idle_coef_set(PLAT_RFM69, 12);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set IDLE coefficient. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static egl_result_t rfm_power_test_run(void)
{
    int8_t power;
    bool pa0_state;
    bool pa1_state;
    bool pa2_state;
    egl_result_t result;

    result = egl_rfm69_power_get(PLAT_RFM69, &power);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get power. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_pa0_get(PLAT_RFM69, &pa0_state);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get PA0 state. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_pa1_get(PLAT_RFM69, &pa1_state);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get PA1 state. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_pa2_get(PLAT_RFM69, &pa2_state);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get PA2 state. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("Power: %d dB", power);
    EGL_LOG_INFO("PA0: %u", pa0_state);
    EGL_LOG_INFO("PA1: %u", pa1_state);
    EGL_LOG_INFO("PA2: %u", pa2_state);

    result = egl_rfm69_power_set(PLAT_RFM69, EGL_RFM69_MIN_POWER_DB);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set power. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_pa0_set(PLAT_RFM69, false);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set PA0. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_pa1_set(PLAT_RFM69, true);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set PA1. Result: %s", EGL_RESULT(result));
        return result;
    }

    result = egl_rfm69_pa2_set(PLAT_RFM69, true);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set PA2. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

void rfm_test_run(void)
{
    egl_result_t result;

    result = rfm_version_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Version test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_bitrate_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Bitrate test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_mode_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Mode test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_data_modul_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Data modul test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_deviation_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Deviation test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_frequency_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Frequency test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_rc_calib_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("RC calibration test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_afc_ctrl_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("AFC control test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_listen_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Listen test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_power_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Power test fail. Result: %s", EGL_RESULT(result));
    }
}

egl_result_t rfm_test_init(void)
{
    egl_result_t result;

    result = egl_rfm69_init(PLAT_RFM69);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to init rfm69 module. Result: %d", EGL_RESULT(result));
        return result;
    }

    return result;
}