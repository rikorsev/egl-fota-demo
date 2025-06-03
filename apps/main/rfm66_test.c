#include <string.h>

#include "egl_lib.h"
#include "plat.h"

static egl_result_t error_hook_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
    egl_log(egl_log_default_get(), EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));

    return result;
}

static egl_result_t rfm_version_test_run(void)
{
    egl_result_t result;
    uint8_t version;

    /* Check version */
    result = egl_rfm66_version_get(PLAT_RFM66, &version);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RFM version: 0x%02x", version);

    return result;
}

static egl_result_t rfm_mode_test_run(void)
{
    egl_result_t result;
    egl_rfm66_mode_t op_mode;
    egl_rfm66_modulation_shaping_t modshap;
    egl_rfm66_modulation_type_t    modtype;

    result = egl_rfm66_mode_get(PLAT_RFM66, &op_mode);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_modulation_shaping_get(PLAT_RFM66, &modshap);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_modulation_type_get(PLAT_RFM66, &modtype);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("OP Mode: %u", op_mode);
    EGL_LOG_INFO("Modulation shaping: %u", modshap);
    EGL_LOG_INFO("Modulation type: %u", modtype);

    result = egl_rfm66_mode_set(PLAT_RFM66, EGL_RFM66_RX_MODE);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_modulation_shaping_set(PLAT_RFM66, EGL_RFM66_MODULATION_SHAPING_1);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_modulation_type_set(PLAT_RFM66, EGL_RFM66_MODULATION_TYPE_OOK);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_bitrate_test_run(void)
{
    egl_result_t result;
    uint32_t bitrate;

    result = egl_rfm66_bitrate_get(PLAT_RFM66, &bitrate);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RFM bitrate: %u", bitrate);

    result = egl_rfm66_bitrate_set(PLAT_RFM66, 115200);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_deviation_test_run(void)
{
    egl_result_t result;
    uint32_t deviation;

    result = egl_rfm66_deviation_get(PLAT_RFM66, &deviation);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Deviation: %u Hz", deviation);

    result = egl_rfm66_deviation_set(PLAT_RFM66, 30000);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_frequency_test_run(void)
{
    egl_result_t result;
    uint32_t frequency;

    result = egl_rfm66_frequency_get(PLAT_RFM66, &frequency);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Frequency: %u Hz", frequency);

    result = egl_rfm66_frequency_set(PLAT_RFM66, 868000000);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_power_test_run(void)
{
    uint8_t power;
    bool low_pn_tx_state;
    egl_result_t result;
    egl_rfm66_pa_select_t select;
    egl_rfm66_power_ramp_t ramp;

    result = egl_rfm66_pa_power_get(PLAT_RFM66, &power);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_pa_select_get(PLAT_RFM66, &select);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_pa_ramp_get(PLAT_RFM66, &ramp);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_low_pn_tx_state_get(PLAT_RFM66, &low_pn_tx_state);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Power raw value: %u", power);
    EGL_LOG_INFO("Power select: %u", select);
    EGL_LOG_INFO("Power ramp: %u", ramp);
    EGL_LOG_INFO("Low PN TX state: %u", low_pn_tx_state);

    result = egl_rfm66_pa_power_set(PLAT_RFM66, 14);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_pa_select_set(PLAT_RFM66, EGL_RFM66_PA_SELECT_PA_BOOST);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_pa_ramp_set(PLAT_RFM66, EGL_RFM66_POWER_RAMP_31_US);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_low_pn_tx_state_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_ocp_test_run(void)
{
    bool ocp_state;
    uint8_t ocp_trim;
    egl_result_t result;

    result = egl_rfm66_ocp_state_get(PLAT_RFM66, &ocp_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ocp_trim_get(PLAT_RFM66, &ocp_trim);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("OCP state: %u", ocp_state);
    EGL_LOG_INFO("1: OCP trim: %u mA", ocp_trim);

    result = egl_rfm66_ocp_state_set(PLAT_RFM66, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ocp_trim_set(PLAT_RFM66, 120);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ocp_trim_get(PLAT_RFM66, &ocp_trim);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("2: OCP trim: %u mA", ocp_trim);

    result = egl_rfm66_ocp_trim_set(PLAT_RFM66, 230);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ocp_trim_get(PLAT_RFM66, &ocp_trim);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("3: OCP trim: %u mA", ocp_trim);

    result = egl_rfm66_ocp_trim_set(PLAT_RFM66, 250);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_lna_test_run(void)
{
    egl_result_t result;
    egl_rfm66_lna_boost_t boost_state;
    egl_rfm66_lna_gain_t gain;

    result = egl_rfm66_lna_boost_state_get(PLAT_RFM66, &boost_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_lna_gain_get(PLAT_RFM66, &gain);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("LNA boost state: %u", boost_state);
    EGL_LOG_INFO("LNA gain: %u", gain);

    result = egl_rfm66_lna_boost_state_set(PLAT_RFM66, EGL_RFM66_LNA_BOOST_ON);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_lna_gain_set(PLAT_RFM66, EGL_RFM66_LNA_GAIN_HIGHEST_MINUS_48_DB);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_rx_config_test_run(void)
{
    bool auto_agc_state;
    bool auto_afc_state;
    bool reset_rx_with_pll_lock;
    bool reset_rx_wo_pll_lock;
    bool reset_rx_on_collision;
    egl_result_t result;
    egl_rfm66_rx_trigger_t trigger;

    result = egl_rfm66_rx_trigger_get(PLAT_RFM66, &trigger);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_auto_agc_state_get(PLAT_RFM66, &auto_agc_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_auto_afc_state_get(PLAT_RFM66, &auto_afc_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_restart_rx_with_pll_lock_state_get(PLAT_RFM66, &reset_rx_with_pll_lock);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_restart_rx_wo_pll_lock_state_get(PLAT_RFM66, &reset_rx_wo_pll_lock);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_restart_rx_on_collision_state_get(PLAT_RFM66, &reset_rx_on_collision);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RX trigger: %u", trigger);
    EGL_LOG_INFO("Auto AGC: %u", auto_agc_state);
    EGL_LOG_INFO("Auto AFC: %u", auto_afc_state);
    EGL_LOG_INFO("RX restart with PLL lock: %u", reset_rx_with_pll_lock);
    EGL_LOG_INFO("RX restart without PLL lock: %u", reset_rx_wo_pll_lock);
    EGL_LOG_INFO("RX restart on collision: %u", reset_rx_on_collision);

    result = egl_rfm66_rx_trigger_set(PLAT_RFM66, EGL_RFM66_RX_TRIGGER_3);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_auto_agc_state_set(PLAT_RFM66, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_auto_afc_state_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_restart_rx_with_pll_lock_state_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_restart_rx_wo_pll_lock_state_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_restart_rx_on_collision_state_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_rssi_config_test_run(void)
{
    int8_t rssi;
    int8_t offset;
    int8_t threshold;
    uint8_t collision;
    egl_result_t result;
    egl_rfm66_rssi_smoothing_t smoothing;

    result = egl_rfm66_rssi_offset_get(PLAT_RFM66, &offset);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rssi_smoothing_get(PLAT_RFM66, &smoothing);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rssi_collision_get(PLAT_RFM66, &collision);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rssi_threshold_get(PLAT_RFM66, &threshold);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rssi_get(PLAT_RFM66, &rssi);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RSSI smoothing: %u", smoothing);
    EGL_LOG_INFO("RSSI collision: %u dB", collision);
    EGL_LOG_INFO("RSSI threshold: %d", threshold);
    EGL_LOG_INFO("RSSI: %d", rssi);
    EGL_LOG_INFO("1: RSSI offset: %d", offset);

    result = egl_rfm66_rssi_smoothing_set(PLAT_RFM66, EGL_RFM66_RSSI_SMOOTHING_256_SAMPLES);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rssi_offset_set(PLAT_RFM66, -16);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rssi_offset_get(PLAT_RFM66, &offset);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("2: RSSI offset: %d", offset);

    result = egl_rfm66_rssi_offset_set(PLAT_RFM66, 15);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rssi_collision_set(PLAT_RFM66, 20);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rssi_threshold_set(PLAT_RFM66, -100);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_bw_test_run(void)
{
    egl_result_t result;
    uint8_t rx_bw_exp;
    egl_rfm66_bw_mant_t rx_bw_mant;
    uint8_t afc_bw_exp;
    egl_rfm66_bw_mant_t afc_bw_mant;

    result = egl_rfm66_rx_bw_exp_get(PLAT_RFM66, &rx_bw_exp);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rx_bw_mant_get(PLAT_RFM66, &rx_bw_mant);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_afc_bw_exp_get(PLAT_RFM66, &afc_bw_exp);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_afc_bw_mant_get(PLAT_RFM66, &afc_bw_mant);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RX BW exp: %u", rx_bw_exp);
    EGL_LOG_INFO("RX BW mant: %u", rx_bw_mant);
    EGL_LOG_INFO("AFC BW exp: %u", afc_bw_exp);
    EGL_LOG_INFO("AFC BW mant: %u", afc_bw_mant);

    result = egl_rfm66_rx_bw_exp_set(PLAT_RFM66, 1);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rx_bw_mant_set(PLAT_RFM66, EGL_RFM66_BW_MANT_20);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_afc_bw_exp_set(PLAT_RFM66, 4);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_afc_bw_mant_set(PLAT_RFM66, EGL_RFM66_BW_MANT_24);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_ook_test_run(void)
{
    bool bit_sync;
    uint8_t fix_thresh;
    egl_result_t result;
    egl_rfm66_ook_thresh_dec_t dec;
    egl_rfm66_ook_thresh_step_t step;
    egl_rfm66_ook_thresh_type_t type;
    egl_rfm66_ook_thresh_avg_filt_t filt;
    egl_rfm66_ook_avg_offset_t offset;

    result = egl_rfm66_ook_peak_thresh_step_get(PLAT_RFM66, &step);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_thresh_type_get(PLAT_RFM66, &type);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_bit_sync_state_get(PLAT_RFM66, &bit_sync);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_thresh_fixed_get(PLAT_RFM66, &fix_thresh);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_thresh_avg_filt_get(PLAT_RFM66, &filt);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_peak_thresh_dec_get(PLAT_RFM66, &dec);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_avg_offset_get(PLAT_RFM66, &offset);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Bit sync state: %u", bit_sync);
    EGL_LOG_INFO("OOK Thresh step: %u", step);
    EGL_LOG_INFO("OOK Thresh type: %u", type);
    EGL_LOG_INFO("OOK Thresh fixed: %u", fix_thresh);
    EGL_LOG_INFO("OOK Thresh dec: %u", dec);
    EGL_LOG_INFO("OOK Thresh avg filt: %u", filt);
    EGL_LOG_INFO("OOK average offset: %u", offset);

    result = egl_rfm66_ook_peak_thresh_step_set(PLAT_RFM66, EGL_RFM66_OOK_THRESH_STEP_2_0_DB);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_thresh_type_set(PLAT_RFM66, EGL_RFM69_OOK_THRESH_TYPE_PEAK);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_bit_sync_state_set(PLAT_RFM66, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_thresh_fixed_set(PLAT_RFM66, 3);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_thresh_avg_filt_set(PLAT_RFM66, EGL_RFM66_OOK_CHIP_RATE_DIV_2P);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_peak_thresh_dec_set(PLAT_RFM66, EGL_RFM66_OOK_THRESH_DEC_ONCE_PER_2_CHIPS);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_ook_avg_offset_set(PLAT_RFM66, EGL_RFM66_OOK_AVERAGE_OFFSET_6_DB);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_afc_fei_test_run(void)
{
    bool afc_auto_clear_state;
    int16_t afc_value;
    int16_t fei_value;
    egl_result_t result;

    result = egl_rfm66_afc_auto_clear_get(PLAT_RFM66, &afc_auto_clear_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_afc_get(PLAT_RFM66, &afc_value);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_fei_get(PLAT_RFM66, &fei_value);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("AFC auto clear state: %u", afc_auto_clear_state);
    EGL_LOG_INFO("AFC value: %d", afc_value);
    EGL_LOG_INFO("FEI value: %d", fei_value);

    result = egl_rfm66_afc_clear(PLAT_RFM66);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_afc_auto_clear_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_agc_start(PLAT_RFM66);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_preamble_test_run(void)
{
    bool state;
    uint8_t tolerance;
    uint16_t preamble;
    egl_result_t result;
    egl_rfm66_preamble_detect_size_t size;

    result = egl_rfm66_preamble_detect_tol_get(PLAT_RFM66, &tolerance);
    EGL_RESULT_CHECK(result);
    
    result = egl_rfm66_preamble_detect_size_get(PLAT_RFM66, &size);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_preamble_detect_state_get(PLAT_RFM66, &state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_preamble_get(PLAT_RFM66, &preamble);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Preamble size: %u", preamble);
    EGL_LOG_INFO("Preamble detect state: %u", state);
    EGL_LOG_INFO("Preamble detect size: %u", size);
    EGL_LOG_INFO("Preamble detect tolerance: %u", tolerance);

    result = egl_rfm66_preamble_set(PLAT_RFM66, 513);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_preamble_detect_tol_set(PLAT_RFM66, 31);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_preamble_detect_size_set(PLAT_RFM66, EGL_RFM66_PREAMBLE_DETECT_SIZE_2_BYTE);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_preamble_detect_state_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_rx_timeout_test_run(void)
{
    egl_result_t result;
    uint8_t rssi_timeout;
    uint8_t preamble_timeout;
    uint8_t signal_sync_timeout;
    uint8_t delay;

    result = egl_rfm66_timeout_rx_rssi_get(PLAT_RFM66, &rssi_timeout);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_timeout_rx_preamble_get(PLAT_RFM66, &preamble_timeout);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_timeout_rx_signal_sync_get(PLAT_RFM66, &signal_sync_timeout);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_timeout_rx_delay_get(PLAT_RFM66, &delay);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RX timeout RSSI: %u", rssi_timeout);
    EGL_LOG_INFO("RX timeout preamble: %u", preamble_timeout);
    EGL_LOG_INFO("RX timeout signal sync: %u", signal_sync_timeout);
    EGL_LOG_INFO("RX delay: %u", delay);

    result = egl_rfm66_timeout_rx_rssi_set(PLAT_RFM66, 10);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_timeout_rx_preamble_set(PLAT_RFM66, 20);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_timeout_rx_signal_sync_set(PLAT_RFM66, 30);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_timeout_rx_delay_set(PLAT_RFM66, 40);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_osc_test_run(void)
{
    egl_result_t result;
    egl_rfm66_clk_out_t out;

    result = egl_rfm66_clk_out_get(PLAT_RFM66, &out);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("CLK out: %u", out);

    result = egl_rfm66_clk_out_set(PLAT_RFM66, EGL_RFM66_CLK_OUT_OFF);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_rc_calibration_start(PLAT_RFM66);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_sync_test_run(void)
{
    bool state;
    uint8_t size;
    egl_result_t result;
    egl_rfm66_preamble_pol_t polarity;
    egl_rfm66_fifo_fill_cond_t cond;
    egl_rfm66_auto_restart_rx_mode_t mode;
    uint8_t sync_buff[EGL_RFM66_SYNC_MAX_SIZE + 1] = { 0 }; /* +1 for 0 termination */
    static const char sync_word[] = "rikorsev";

    result = egl_rfm66_sync_size_get(PLAT_RFM66, &size);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_fifo_fill_cond_get(PLAT_RFM66, &cond);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_sync_state_get(PLAT_RFM66, &state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_preamble_polarity_get(PLAT_RFM66, &polarity);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_auto_restart_rx_mode_get(PLAT_RFM66, &mode);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_sync_get(PLAT_RFM66, sync_buff, &size);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Sync state: %u", state);
    EGL_LOG_INFO("Sync size: %u", size);
    EGL_LOG_INFO("Preamble polarity: %u", polarity);
    EGL_LOG_INFO("Fifo fill condition: %u", cond);
    EGL_LOG_INFO("Auto restart RX mode: %u", mode);
    EGL_LOG_INFO("Sync bytes: %02x %02x %02x %02x %02x %02x %02x %02x", sync_buff[0],
                                                                        sync_buff[1],
                                                                        sync_buff[2],
                                                                        sync_buff[3],
                                                                        sync_buff[4],
                                                                        sync_buff[5],
                                                                        sync_buff[6],
                                                                        sync_buff[7]);
    EGL_LOG_INFO("Sync string: %s", sync_buff);

    result = egl_rfm66_sync_size_set(PLAT_RFM66, 8);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_fifo_fill_cond_set(PLAT_RFM66, EGL_RFM66_FIFO_FILL_COND_FIFO_COND_SET);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_sync_state_set(PLAT_RFM66, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_preamble_polarity_set(PLAT_RFM66, EGL_RFM66_PREAMBLE_POL_55);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_auto_restart_rx_mode_set(PLAT_RFM66, EGL_RFM66_AUTO_RESTART_RX_MODE_ON_WO_PLL);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_sync_set(PLAT_RFM66, (uint8_t *)sync_word, strlen(sync_word));
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_packet_config_test_run(void)
{
    bool crc_autoclear_state;
    bool crc_state;
    bool beacon_state;
    bool io_home_state;
    bool io_home_power_frame_state;
    uint16_t length;
    egl_result_t result;
    egl_rfm66_dc_free_t type;
    egl_rfm66_data_mode_t mode;
    egl_rfm66_crc_whitening_t whitening;
    egl_rfm66_packet_format_t format;
    egl_rfm66_address_filtering_t filtering;

    result = egl_rfm66_crc_whitening_type_get(PLAT_RFM66, &whitening);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_address_filtering_get(PLAT_RFM66, &filtering);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_crc_autoclear_state_get(PLAT_RFM66, &crc_autoclear_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_crc_state_get(PLAT_RFM66, &crc_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_dc_free_get(PLAT_RFM66, &type);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_packet_format_get(PLAT_RFM66, &format);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_packet_length_get(PLAT_RFM66, &length);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_beacon_mode_get(PLAT_RFM66, &beacon_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_io_home_power_frame_state_get(PLAT_RFM66, &io_home_power_frame_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_io_home_state_get(PLAT_RFM66, &io_home_state);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_data_mode_get(PLAT_RFM66, &mode);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("CRC whitening type: %u", whitening);
    EGL_LOG_INFO("Address filtering: %u", filtering);
    EGL_LOG_INFO("CRC autoclear state: %u", crc_autoclear_state);
    EGL_LOG_INFO("CRC state: %u", crc_state);
    EGL_LOG_INFO("DC free: %u", type);
    EGL_LOG_INFO("Packet format: %u", format);
    EGL_LOG_INFO("Packet length: %u", length);
    EGL_LOG_INFO("Beacon state: %u", beacon_state);
    EGL_LOG_INFO("IO home power frame: %u", io_home_power_frame_state);
    EGL_LOG_INFO("IO home state: %u", io_home_state);
    EGL_LOG_INFO("Data mode: %u", mode);

    result = egl_rfm66_crc_whitening_type_set(PLAT_RFM66, EGL_RFM66_CRC_WHITENING_TYPE_IBM);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_address_filtering_set(PLAT_RFM66, EGK_RFM66_ADDRESS_FILTERING_MATCH_NODE_AND_BROADCAST_ADDRESS);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_crc_autoclear_state_set(PLAT_RFM66, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_crc_state_set(PLAT_RFM66, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_dc_free_set(PLAT_RFM66, EGL_RFM66_DC_FREE_ENCODING_WHITENING);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_packet_format_set(PLAT_RFM66, EGL_RFM66_PACKET_FORMAT_FIXED);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_packet_length_set(PLAT_RFM66, 513);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_beacon_mode_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_io_home_power_frame_state_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_io_home_state_set(PLAT_RFM66, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_data_mode_set(PLAT_RFM66, EGL_RFM66_DATA_MODE_CONTINIOUS);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_address_test_run(void)
{
    uint8_t node_address;
    uint8_t broadcast_address;
    egl_result_t result;

    result = egl_rfm66_node_address_get(PLAT_RFM66, &node_address);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_broadcast_address_get(PLAT_RFM66, &broadcast_address);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Node address: %u", node_address);
    EGL_LOG_INFO("Broadcast address: %u", broadcast_address);

    result = egl_rfm66_node_address_set(PLAT_RFM66, 111);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_broadcast_address_set(PLAT_RFM66, 222);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_fifo_thresh_test_run(void)
{
    uint8_t thresh;
    egl_result_t result;
    egl_rfm66_tx_start_condition_t cond;

    result = egl_rfm66_fifo_thresh_get(PLAT_RFM66, &thresh);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_tx_start_condition_get(PLAT_RFM66, &cond);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("Fifo threshold: %u", thresh);
    EGL_LOG_INFO("TX start condition: %u", cond);

    result = egl_rfm66_fifo_thresh_set(PLAT_RFM66, 31);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_tx_start_condition_set(PLAT_RFM66, EGL_RFM66_TX_START_CONDITION_FIFO_EMPTY);
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

    result = rfm_mode_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Mode test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_bitrate_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Bitrate test fail. Result: %s", EGL_RESULT(result));
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

    result = rfm_rx_config_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("RX config test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_rssi_config_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("RSSI config test fail. Result: %s", EGL_RESULT(result));
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

    result = rfm_afc_fei_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("AFC test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_preamble_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Preamble test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_rx_timeout_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("RX timeout test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_osc_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("OSC test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_sync_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Sync config test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_packet_config_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Packet config test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_address_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Address test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_fifo_thresh_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("FIFO thresh test fail. Result: %s", EGL_RESULT(result));
    }
}

egl_result_t rfm_test_init(void)
{
    egl_result_t result;

    result = egl_rfm66_init(PLAT_RFM66);
    EGL_RESULT_CHECK(result);

    return result;
}