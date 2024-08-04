#ifndef PLAT_RFM_TX_LED_PIN_H
#define PLAT_RFM_TX_LED_PIN_H

#define PLAT_RFM_TX_LED plat_rfm_tx_led_pin_get()

egl_pio_t *plat_rfm_tx_led_pin_get(void);

#endif