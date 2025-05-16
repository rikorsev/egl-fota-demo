#include <string.h>

#include "egl_lib.h"
#include "plat.h"

static egl_result_t rfm_version_test_run(void)
{
    egl_result_t result;
    uint8_t version;

    /* Check version */
    result = egl_rfm69_read_byte(PLAT_RFM69, EGL_RFM69_REG_VERSION, &version);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RFM version: 0x%02x", version);

    return result;
}

static egl_result_t rfm_bitrate_test_run(void)
{
    egl_result_t result;
    uint32_t bitrate;

    result = egl_rfm69_bitrate_get(PLAT_RFM69, &bitrate);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RFM bitrate: %u", bitrate);

    result = egl_rfm69_bitrate_set(PLAT_RFM69, 115200);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_mode_test_run(void)
{
    egl_result_t result;
    egl_rfm69_mode_t mode;

    result = egl_rfm69_mode_get(PLAT_RFM69, &mode);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RFM mode: %u", mode);

    result = egl_rfm69_mode_set(PLAT_RFM69, EGL_RFM69_RX_MODE);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_data_modul_test_run(void)
{
    egl_result_t result;
    egl_rfm69_modulation_shaping_t modshap;
    egl_rfm69_modulation_type_t    modtype;
    egl_rfm69_data_mode_t          datamode;

    result = egl_rfm69_modulation_shaping_get(PLAT_RFM69, &modshap);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_modulation_type_get(PLAT_RFM69, &modtype);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_data_mode_get(PLAT_RFM69, &datamode);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Modulation shaping: %u", modshap);
    EGL_LOG_INFO("Modulation type: %u", modtype);
    EGL_LOG_INFO("Data mode: %u", datamode);

    result = egl_rfm69_modulation_shaping_set(PLAT_RFM69, EGL_RFM69_MODULATION_SHAPING_1);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_modulation_type_set(PLAT_RFM69, EGL_RFM69_MODULATION_TYPE_OOK);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_data_mode_set(PLAT_RFM69, EGL_RFM69_DATA_MODE_CONTINIOUS_W_SYNC);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_deviation_test_run(void)
{
    egl_result_t result;
    uint32_t deviation;

    result = egl_rfm69_deviation_get(PLAT_RFM69, &deviation);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Deviation: %u", deviation);

    result = egl_rfm69_deviation_set(PLAT_RFM69, 30000);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_frequency_test_run(void)
{
    egl_result_t result;
    uint32_t frequency;

    result = egl_rfm69_frequency_get(PLAT_RFM69, &frequency);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Frequency: %u", frequency);

    result = egl_rfm69_frequency_set(PLAT_RFM69, 868000000);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_rc_calib_test_run(void)
{
    egl_result_t result;
    egl_rfm69_rc_calib_state_t state;

    result = egl_rfm69_mode_set(PLAT_RFM69, EGL_RFM69_STANDBY_MODE);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_rc_calib_state_get(PLAT_RFM69, &state);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RC calib state: %u", state);

    result = egl_rfm69_rc_calib_start(PLAT_RFM69);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RC calibration start");

    do
    {
        result = egl_rfm69_rc_calib_state_get(PLAT_RFM69, &state);
        EGL_RESULT_CHECK(result);
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
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("AFC routine: %u", routine);

    result = egl_rfm69_afc_routine_set(PLAT_RFM69, EGL_RFM69_AFC_ROUTINE_IMPROVED);
    EGL_RESULT_CHECK(result);

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
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_criteria_get(PLAT_RFM69, &criteria);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_rx_resolution_get(PLAT_RFM69, &rx_resolution);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_idle_resolution_get(PLAT_RFM69, &idle_resolution);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_rx_coef_get(PLAT_RFM69, &rx_coef);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_idle_coef_get(PLAT_RFM69, &idle_coef);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Listen end action: %u", end_action);
    EGL_LOG_INFO("Listen criteria: %u", criteria);
    EGL_LOG_INFO("Listen RX resolution: %u", rx_resolution);
    EGL_LOG_INFO("Listen IDLE resolution: %u", idle_resolution);
    EGL_LOG_INFO("Listen RX coefficient: %u", rx_coef);
    EGL_LOG_INFO("Listen IDLE coefficient: %u", idle_coef);

    result = egl_rfm69_listen_end_set(PLAT_RFM69, EGL_RFM69_LISTEN_STAY_RX);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_criteria_set(PLAT_RFM69, EGL_RFM69_LISTEN_CRITERIA_ABOVE_RSSI_AND_SYNC_ADDR);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_rx_resolution_set(PLAT_RFM69, EGL_RFM69_LISTEN_RESOL_4_1MS);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_idle_resolution_set(PLAT_RFM69, EGL_RFM69_LISTEN_RESOL_262MS);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_rx_coef_set(PLAT_RFM69, 11);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_listen_idle_coef_set(PLAT_RFM69, 12);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_power_test_run(void)
{
    int8_t power;
    bool pa0_state;
    bool pa1_state;
    bool pa2_state;
    egl_result_t result;
    egl_rfm69_power_ramp_t ramp;

    result = egl_rfm69_power_get(PLAT_RFM69, &power);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa0_get(PLAT_RFM69, &pa0_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa1_get(PLAT_RFM69, &pa1_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa2_get(PLAT_RFM69, &pa2_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_power_ramp_get(PLAT_RFM69, &ramp);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Power: %d dB", power);
    EGL_LOG_INFO("PA0: %u", pa0_state);
    EGL_LOG_INFO("PA1: %u", pa1_state);
    EGL_LOG_INFO("PA2: %u", pa2_state);
    EGL_LOG_INFO("Ramp: %u", ramp);

    result = egl_rfm69_power_set(PLAT_RFM69, EGL_RFM69_MIN_POWER_DB);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa0_set(PLAT_RFM69, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa1_set(PLAT_RFM69, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa2_set(PLAT_RFM69, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_power_ramp_set(PLAT_RFM69, EGL_RFM69_POWER_RAMP_2_MS);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_ocp_test_run(void)
{
    bool ocp_state;
    uint8_t ocp_trim;
    egl_result_t result;

    result = egl_rfm69_ocp_trim_get(PLAT_RFM69, &ocp_trim);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ocp_state_get(PLAT_RFM69, &ocp_state);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("OCP state: %u", ocp_state);
    EGL_LOG_INFO("OCP trim: %u mA", ocp_trim);

    result = egl_rfm69_ocp_trim_set(PLAT_RFM69, 120);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ocp_state_set(PLAT_RFM69, false);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_lna_test_run(void)
{
    egl_result_t result;
    egl_rfm69_lna_gain_t select_gain;
    egl_rfm69_lna_gain_t current_gain;
    egl_rfm69_lna_zin_t zin;

    result = egl_rfm69_lna_select_gain_get(PLAT_RFM69, &select_gain);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_lna_current_gain_get(PLAT_RFM69, &current_gain);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_lna_zin_get(PLAT_RFM69, &zin);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("LNA gain select: %u", select_gain);
    EGL_LOG_INFO("LNA gain current: %u", current_gain);
    EGL_LOG_INFO("LNA Zin: %u", zin);

    result = egl_rfm69_lna_select_gain_set(PLAT_RFM69, EGL_RFM69_LNA_GAIN_HIGHEST_MINUS_48_DB);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_lna_zin_set(PLAT_RFM69, EGL_RFM69_LNA_ZIN_200_OHMS);
    EGL_RESULT_CHECK(result);

    return result;
}


static egl_result_t rfm_bw_test_run(void)
{
    egl_result_t result;
    uint8_t rx_bw_exp;
    egl_rfm69_bw_mant_t rx_bw_mant;
    uint8_t rx_dcc_freq;
    uint8_t afc_bw_exp;
    egl_rfm69_bw_mant_t afc_bw_mant;
    uint8_t afc_dcc_freq;

    result = egl_rfm69_rx_bw_exp_get(PLAT_RFM69, &rx_bw_exp);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_rx_bw_mant_get(PLAT_RFM69, &rx_bw_mant);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_rx_dcc_freq_get(PLAT_RFM69, &rx_dcc_freq);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_bw_exp_get(PLAT_RFM69, &afc_bw_exp);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_bw_mant_get(PLAT_RFM69, &afc_bw_mant);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_dcc_freq_get(PLAT_RFM69, &afc_dcc_freq);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RX BW exp: %u", rx_bw_exp);
    EGL_LOG_INFO("RX BW mant: %u", rx_bw_mant);
    EGL_LOG_INFO("RX DCC freq: %u", rx_dcc_freq);
    EGL_LOG_INFO("AFC BW exp: %u", afc_bw_exp);
    EGL_LOG_INFO("AFC BW mant: %u", afc_bw_mant);
    EGL_LOG_INFO("AFC DCC freq: %u", afc_dcc_freq);

    result = egl_rfm69_rx_bw_exp_set(PLAT_RFM69, 1);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_rx_bw_mant_set(PLAT_RFM69, EGL_RFM69_BW_MANT_20);
    if(result != EGL_SUCCESS)
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_rx_dcc_freq_set(PLAT_RFM69, 3);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_bw_exp_set(PLAT_RFM69, 4);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_bw_mant_set(PLAT_RFM69, EGL_RFM69_BW_MANT_24);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_dcc_freq_set(PLAT_RFM69, 6);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_ook_test_run(void)
{
    egl_result_t result;
    egl_rfm_ook_thresh_dec_t dec;
    egl_rfm69_ook_thresh_step_t step;
    egl_rfm69_ook_thresh_type_t type;
    egl_rfm69_ook_thresh_avg_filt_t filt;
    uint8_t fixed_thrash;

    result = egl_rfm69_ook_peak_thresh_dec_get(PLAT_RFM69, &dec);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ook_peak_thresh_step_get(PLAT_RFM69, &step);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ook_thresh_type_get(PLAT_RFM69, &type);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ook_thresh_avg_filt_get(PLAT_RFM69, &filt);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ook_thresh_fixed_get(PLAT_RFM69, &fixed_thrash);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("OOK Thresh dec: %u", dec);
    EGL_LOG_INFO("OOK Thresh step: %u", step);
    EGL_LOG_INFO("OOK Thresh type: %u", type);
    EGL_LOG_INFO("OOK Thresh avg filt: %u", filt);
    EGL_LOG_INFO("OOK Thresh fixed: %u", fixed_thrash);

    result = egl_rfm69_ook_peak_thresh_dec_set(PLAT_RFM69, EGL_RFM69_OOK_THRESH_DEC_ONCE_PER_2_CHIPS);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ook_peak_thresh_step_set(PLAT_RFM69, EGL_RFM69_OOK_THRESH_STEP_2_0_DB);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ook_thresh_type_set(PLAT_RFM69, EGL_RFM69_OOK_THRESH_TYPE_PEAK);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ook_thresh_avg_filt_set(PLAT_RFM69, EGL_RFM69_OOK_CHIP_RATE_DIV_2P);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_ook_thresh_fixed_set(PLAT_RFM69, 3);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_afc_test_run(void)
{
    egl_result_t result;
    bool afc_state;
    bool afc_auto_start_state;
    bool afc_auto_clear_state;
    int16_t afc_value;

    result = egl_rfm69_afc_state_get(PLAT_RFM69, &afc_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_auto_start_get(PLAT_RFM69, &afc_auto_start_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_auto_clear_get(PLAT_RFM69, &afc_auto_clear_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_get(PLAT_RFM69, &afc_value);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("AFC state: %u", afc_state);
    EGL_LOG_INFO("AFC value: %d Hz", afc_value);
    EGL_LOG_INFO("1: AFC auto start state: %u", afc_auto_start_state);
    EGL_LOG_INFO("1: AFC auto clear state: %u", afc_auto_clear_state);

    result = egl_rfm69_afc_auto_start_set(PLAT_RFM69, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_auto_clear_set(PLAT_RFM69, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_auto_start_get(PLAT_RFM69, &afc_auto_start_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_auto_clear_get(PLAT_RFM69, &afc_auto_clear_state);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("2: AFC auto start state: %u", afc_auto_start_state);
    EGL_LOG_INFO("2: AFC auto clear state: %u", afc_auto_clear_state);

    result = egl_rfm69_afc_auto_start_set(PLAT_RFM69, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_auto_clear_set(PLAT_RFM69, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_auto_start_get(PLAT_RFM69, &afc_auto_start_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_auto_clear_get(PLAT_RFM69, &afc_auto_clear_state);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("3: AFC auto start state: %u", afc_auto_start_state);
    EGL_LOG_INFO("3: AFC auto clear state: %u", afc_auto_clear_state);

    result = egl_rfm69_afc_start(PLAT_RFM69);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("AFC started");

    do
    {
        EGL_LOG_INFO("AFC in progress...");
        result = egl_rfm69_afc_state_get(PLAT_RFM69, &afc_state);
        EGL_RESULT_CHECK(result);
    }while(afc_state != true);

    EGL_LOG_INFO("AFC complete");

    result = egl_rfm69_afc_clear(PLAT_RFM69);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_fei_test_run(void)
{
    egl_result_t result;
    bool fei_state;
    int16_t fei_value;

    result = egl_rfm69_fei_state_get(PLAT_RFM69, &fei_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_fei_get(PLAT_RFM69, &fei_value);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("FEI state: %u", fei_state);
    EGL_LOG_INFO("FEI value: %d Hz", fei_value);

#if 0
    result = egl_rfm69_fei_start(PLAT_RFM69);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("FEI start");

    do
    {
        EGL_LOG_INFO("FEI in progress...");
        result = egl_rfm69_fei_state_get(PLAT_RFM69, &fei_state);
        EGL_RESULT_CHECK(result);
    }while(fei_state != true);

    EGL_LOG_INFO("FEI complete");
#endif

    return result;
}

static egl_result_t rfm_rssi_test_run(void)
{
    bool state;
    egl_result_t result;
    int8_t value;
    int8_t thresh;

    result = egl_rfm69_rssi_state_get(PLAT_RFM69, &state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_rssi_get(PLAT_RFM69, &value);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_rssi_thresh_get(PLAT_RFM69, &thresh);
    EGL_RESULT_CHECK(result);


    EGL_LOG_INFO("RSSI state: %u", state);
    EGL_LOG_INFO("RSSI value: %d dB", value);
    EGL_LOG_INFO("RSSI thresh: %d dB", thresh);

    result = egl_rfm69_rssi_thresh_set(PLAT_RFM69, -100);
    EGL_RESULT_CHECK(result);

#if 0
    result = egl_rfm69_rssi_start(PLAT_RFM69);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RSSI measure start");

    do
    {
        EGL_LOG_INFO("RSSI measure in progress...");
        result = egl_rfm69_rssi_state_get(PLAT_RFM69, &state);
        EGL_RESULT_CHECK(result);
    }while(state != true);

    EGL_LOG_INFO("RSSI measure complete");
#endif

    return result;
}

static egl_result_t error_hook_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
    egl_log(egl_log_default_get(), EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));

    return result;
}

static egl_result_t rfm_dio_test_run(void)
{
    egl_result_t result;
    egl_rfm69_dio_mode_t dio0;
    egl_rfm69_dio_mode_t dio1;
    egl_rfm69_dio_mode_t dio2;
    egl_rfm69_dio_mode_t dio3;
    egl_rfm69_dio_mode_t dio4;
    egl_rfm69_dio_mode_t dio5;
    egl_rfm69_clk_out_t clk_out;

    result = egl_rfm69_dio0_mode_get(PLAT_RFM69, &dio0);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio1_mode_get(PLAT_RFM69, &dio1);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio2_mode_get(PLAT_RFM69, &dio2);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio3_mode_get(PLAT_RFM69, &dio3);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio4_mode_get(PLAT_RFM69, &dio4);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio5_mode_get(PLAT_RFM69, &dio5);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_clk_out_get(PLAT_RFM69, &clk_out);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("DIO0 mode: %u", dio0);
    EGL_LOG_INFO("DIO1 mode: %u", dio1);
    EGL_LOG_INFO("DIO2 mode: %u", dio2);
    EGL_LOG_INFO("DIO3 mode: %u", dio3);
    EGL_LOG_INFO("DIO4 mode: %u", dio4);
    EGL_LOG_INFO("DIO5 mode: %u", dio5);
    EGL_LOG_INFO("CLK OUT mode: %u", clk_out);

    result = egl_rfm69_dio0_mode_set(PLAT_RFM69, EGL_RFM69_DIO_MODE_1);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio1_mode_set(PLAT_RFM69, EGL_RFM69_DIO_MODE_2);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio2_mode_set(PLAT_RFM69, EGL_RFM69_DIO_MODE_3);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio3_mode_set(PLAT_RFM69, EGL_RFM69_DIO_MODE_1);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio4_mode_set(PLAT_RFM69, EGL_RFM69_DIO_MODE_2);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dio5_mode_set(PLAT_RFM69, EGL_RFM69_DIO_MODE_3);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_clk_out_set(PLAT_RFM69, EGL_RFM69_CLK_OUT_FXOSC_DIV_16);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_flags_test_run(void)
{
    egl_result_t result;
    egl_rfm69_irq_flags_t flags;

    result = egl_rfm69_flags_get(PLAT_RFM69, &flags);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("FLAGS raw: %04x", flags.raw);
    EGL_LOG_INFO("FLAGS sync_addr_match: %u", flags.bitfield.sync_addr_match);
    EGL_LOG_INFO("FLAGS auto_mode: %u", flags.bitfield.auto_mode);
    EGL_LOG_INFO("FLAGS timeout: %u", flags.bitfield.timeout);
    EGL_LOG_INFO("FLAGS rssi: %u", flags.bitfield.rssi);
    EGL_LOG_INFO("FLAGS pll_lock: %u", flags.bitfield.pll_lock);
    EGL_LOG_INFO("FLAGS tx_ready: %u", flags.bitfield.tx_ready);
    EGL_LOG_INFO("FLAGS rx_ready: %u", flags.bitfield.rx_ready);
    EGL_LOG_INFO("FLAGS mode_ready: %u", flags.bitfield.mode_ready);
    EGL_LOG_INFO("FLAGS reserved: %u", flags.bitfield.reserved);
    EGL_LOG_INFO("FLAGS crc_ok: %u", flags.bitfield.crc_ok);
    EGL_LOG_INFO("FLAGS payload_ready: %u", flags.bitfield.payload_ready);
    EGL_LOG_INFO("FLAGS packet_sent: %u", flags.bitfield.packet_sent);
    EGL_LOG_INFO("FLAGS fifo_overrun: %u", flags.bitfield.fifo_overrun);
    EGL_LOG_INFO("FLAGS fifo_level: %u", flags.bitfield.fifo_level);
    EGL_LOG_INFO("FLAGS fifo_not_empty: %u", flags.bitfield.fifo_not_empty);
    EGL_LOG_INFO("FLAGS fifo_full: %u", flags.bitfield.fifo_full);

    return result;
}

static egl_result_t rfm_timeout_test_run(void)
{
    egl_result_t result;
    uint8_t rx_start_tout;
    uint8_t rssi_thresh_tout;

    result = egl_rfm69_timeout_rx_start_get(PLAT_RFM69, &rx_start_tout);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_timeout_rssi_thresh_get(PLAT_RFM69, &rssi_thresh_tout);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("TIMEOUT RX start: %u", rx_start_tout);
    EGL_LOG_INFO("TIMEOUT RSSI thresh: %u", rssi_thresh_tout);

    result = egl_rfm69_timeout_rx_start_set(PLAT_RFM69, 123);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_timeout_rssi_thresh_set(PLAT_RFM69, 231);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_preamble_test_run(void)
{
    egl_result_t result;
    uint16_t preamble_len;

    result = egl_rfm69_preamble_get(PLAT_RFM69, &preamble_len);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Preamble len: %u", preamble_len);

    result = egl_rfm69_preamble_set(PLAT_RFM69, 300);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_sync_test_run(void)
{
    uint8_t sync_tol;
    uint8_t sync_size;
    bool sync_state;
    egl_result_t result;
    egl_rfm69_fifo_fill_cont_t fill_cond;
    uint8_t sync_buff[EGL_RFM69_SYNC_MAX_SIZE + 1] = { 0 }; /* +1 for 0 termination */
    char *sync_str = "rikorsev";

    result = egl_rfm69_sync_tol_get(PLAT_RFM69, &sync_tol);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_fifo_fill_cond_get(PLAT_RFM69, &fill_cond);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_sync_state_get(PLAT_RFM69, &sync_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_sync_get(PLAT_RFM69, sync_buff, &sync_size);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Sync state: %u", sync_state);
    EGL_LOG_INFO("Sync tol: %u", sync_tol);
    EGL_LOG_INFO("Sync size: %u", sync_size);
    EGL_LOG_INFO("Fifo fill condition: %u", fill_cond);
    EGL_LOG_INFO("Sync bytes: %02x %02x %02x %02x %02x %02x %02x %02x", sync_buff[0],
                                                                        sync_buff[1],
                                                                        sync_buff[2],
                                                                        sync_buff[3],
                                                                        sync_buff[4],
                                                                        sync_buff[5],
                                                                        sync_buff[6],
                                                                        sync_buff[7]);
    EGL_LOG_INFO("Sync string: %s", sync_buff);

    result = egl_rfm69_sync_tol_set(PLAT_RFM69, 7);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_fifo_fill_cond_set(PLAT_RFM69, EGL_RFM69_FIFO_FILL_COND_AS_LONG_AS_SET);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_sync_state_set(PLAT_RFM69, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_sync_set(PLAT_RFM69, (uint8_t *)sync_str, strlen(sync_str));
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_packet_config_test_run(void)
{
    egl_result_t result;
    egl_rfm69_address_filtering_t filtering;
    egl_rfm69_packet_format_t packet_format;
    egl_rfm69_dc_free_t dc_free_type;
    bool crc_auto_clear;
    bool crc_check;
    bool aes_state;
    bool rx_auto_restart;
    uint8_t interpacket_delay;

    result = egl_rfm69_address_filtering_get(PLAT_RFM69, &filtering);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_crc_auto_clear_state_get(PLAT_RFM69, &crc_auto_clear);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_crc_check_state_get(PLAT_RFM69, &crc_check);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dc_free_get(PLAT_RFM69, &dc_free_type);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_packet_format_get(PLAT_RFM69, &packet_format);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_aes_state_get(PLAT_RFM69, &aes_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_auto_rx_restart_state_get(PLAT_RFM69, &rx_auto_restart);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_interpacket_delay_get(PLAT_RFM69, &interpacket_delay);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Packet filtering: %u", filtering);
    EGL_LOG_INFO("Packet format: %u", packet_format);
    EGL_LOG_INFO("Packet DC free type: %u", dc_free_type);
    EGL_LOG_INFO("Packet CRC autoclear: %u", crc_auto_clear);
    EGL_LOG_INFO("Packet CRC state: %u", crc_check);
    EGL_LOG_INFO("Packet AES state: %u", aes_state);
    EGL_LOG_INFO("Packet RX autorestart: %u", rx_auto_restart);
    EGL_LOG_INFO("Packet RX delay: %u", interpacket_delay);

    result = egl_rfm69_address_filtering_set(PLAT_RFM69, EGL_RFM69_ADDRESS_GILTERING_NODE_AND_BROADCAST);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_crc_auto_clear_state_set(PLAT_RFM69, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_crc_check_state_set(PLAT_RFM69, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dc_free_set(PLAT_RFM69, EGL_RFM69_DC_FREE_MANCHESTER);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_packet_format_set(PLAT_RFM69, EGL_RFM69_PACKET_FORMAT_VARIABLE_LENGTH);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_aes_state_set(PLAT_RFM69, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_auto_rx_restart_state_set(PLAT_RFM69, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_interpacket_delay_set(PLAT_RFM69, 15);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_rx_restart(PLAT_RFM69);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_payload_length_test_run(void)
{
    egl_result_t result;
    uint8_t payload_len;

    result = egl_rfm69_payload_length_get(PLAT_RFM69, &payload_len);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Payload length: %u", payload_len);

    result = egl_rfm69_payload_length_set(PLAT_RFM69, 255);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_address_test_run(void)
{
    egl_result_t result;
    uint8_t address_node;
    uint8_t address_broadcast;

    result = egl_rfm69_node_address_get(PLAT_RFM69, &address_node);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_broadcast_address_get(PLAT_RFM69, &address_broadcast);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Address node: %u", address_node);
    EGL_LOG_INFO("Address broadcast: %u", address_broadcast);

    result = egl_rfm69_node_address_set(PLAT_RFM69, 11);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_broadcast_address_set(PLAT_RFM69, 22);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_auto_modes_test_run(void)
{
    egl_result_t result;
    egl_rfm69_intermediate_mode_t mode;
    egl_rfm69_exit_condition_t exit_cond;
    egl_rfm69_enter_condition_t enter_cond;

    result = egl_rfm69_intermediate_mode_get(PLAT_RFM69, &mode);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_exit_condition_get(PLAT_RFM69, &exit_cond);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_enter_condition_get(PLAT_RFM69, &enter_cond);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Intermediate mode: %u", mode);
    EGL_LOG_INFO("Exit condition: %u", exit_cond);
    EGL_LOG_INFO("Enter condition: %u", enter_cond);

    result = egl_rfm69_intermediate_mode_set(PLAT_RFM69, EGL_RFM69_INTERMEDIATE_MODE_TX);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_exit_condition_set(PLAT_RFM69, EGL_RFM69_EXIT_CONDITION_FALL_EDGE_FIFO_NOT_EMPTY);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_enter_condition_set(PLAT_RFM69, EGL_RFM69_ENTER_CONDITION_RISE_CRC_OK);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_fifo_thresh_thresh_run(void)
{
    egl_result_t result;
    uint8_t fifo_thresh;
    egl_rfm69_tx_start_cond_t tx_start_cond;

    result = egl_rfm69_fifo_thresh_get(PLAT_RFM69, &fifo_thresh);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_tx_start_cond_get(PLAT_RFM69, &tx_start_cond);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("FIFO thresh: %u", fifo_thresh);
    EGL_LOG_INFO("TX start cond: %u", tx_start_cond);

    result = egl_rfm69_fifo_thresh_set(PLAT_RFM69, 127);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_tx_start_cond_set(PLAT_RFM69, EGL_RFM69_TX_START_CONDITION_FIFO_NOT_EMPTY);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_aes_test_run(void)
{
    uint8_t aes_key[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    return  egl_rfm69_aes_key_set(PLAT_RFM69, aes_key, sizeof(aes_key));
}

static egl_result_t rfm_temp_test_result(void)
{
    egl_result_t result;
    bool state;
    int8_t temp;

    result = egl_rfm69_temp_meas_start(PLAT_RFM69);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Temp measurement start");

    do
    {
        EGL_LOG_INFO("Temp measurement in progress...");
        result = egl_rfm69_temp_meas_state_get(PLAT_RFM69, &state);
        EGL_RESULT_CHECK(result);
    }while(state == true);

    EGL_LOG_INFO("Temp measurement complete");

    result = egl_rfm69_temp_get(PLAT_RFM69, &temp);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Temp: %d", temp);

    return result;
}

static egl_result_t rfm_test_reg_test_run(void)
{
    uint32_t afc_offset;
    egl_result_t result;
    egl_rfm69_sensitivity_mode_t sens_mode;
    egl_rfm69_pa1_mode_t pa1_mode;
    egl_rfm69_pa2_mode_t pa2_mode;
    egl_rfm69_dagc_mode_t dagc_mode;

    result = egl_rfm69_sensitivity_mode_get(PLAT_RFM69, &sens_mode);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa1_mode_get(PLAT_RFM69, &pa1_mode);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa2_mode_get(PLAT_RFM69, &pa2_mode);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dagc_mode_get(PLAT_RFM69, &dagc_mode);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_offset_get(PLAT_RFM69, &afc_offset);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Test sensetivity mode: %u (0x%02x)", sens_mode, sens_mode);
    EGL_LOG_INFO("Test PA1 mode: %u (0x%02x)", pa1_mode, pa1_mode);
    EGL_LOG_INFO("Test PA2 mode: %u (0x%02x)", pa2_mode, pa2_mode);
    EGL_LOG_INFO("Test DAGC mode: %u (0x%02x)", dagc_mode, dagc_mode);
    EGL_LOG_INFO("Test AFC offset: %u Hz", afc_offset);

    result = egl_rfm69_sensitivity_mode_set(PLAT_RFM69, EGL_RFM69_SENSITIVITY_MODE_HIGH);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa1_mode_set(PLAT_RFM69, EGL_RFM69_PA1_MODE_PLUS_13_DBM);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_pa2_mode_set(PLAT_RFM69, EGL_RFM69_PA2_MODE_PLUS_13_DBM);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_dagc_mode_set(PLAT_RFM69, EGL_RFM69_DAGC_MODE_IMPROVED_MARGIN2);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_afc_offset_set(PLAT_RFM69, 124440);
    EGL_RESULT_CHECK(result);

    return result;
}

void rfm_test_run(void)
{
    egl_result_t result;
    egl_result_error_hook_t error_hook = { error_hook_func };

    egl_result_error_hook_set(&error_hook);

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

    result = rfm_ocp_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("OCP test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_lna_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("LNA test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_bw_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("BW test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_ook_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("OOK test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_afc_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("AFC test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_fei_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("FEI test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_rssi_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("RSSI test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_dio_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("DIO test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_flags_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Flags test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_timeout_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Timeout test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_preamble_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Preamble test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_sync_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Sync test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_packet_config_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Packet config test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_payload_length_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Payload length test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_address_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Address test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_auto_modes_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Auto modes test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_fifo_thresh_thresh_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fifo thresh test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_aes_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("AES test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_temp_test_result();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Temp test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_test_reg_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Test reg test fail. Result: %s", EGL_RESULT(result));
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