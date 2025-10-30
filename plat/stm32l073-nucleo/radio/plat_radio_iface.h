#ifndef PLAT_RADIO_IFACE_H
#define PLAT_RADIO_IFACE_H

#include "plat_radio_sw1.h"
#include "plat_radio_sw2.h"

#define RADIO plat_radio_iface_get()

#define RADIO_FREQ        (915000000U)
#define RADIO_DEVIATION   (35000U)
#define RADIO_BITRATE     (19200U)
#define RADIO_PREAMBLE    (16U)
#define RADIO_NODE_ADDR   (192U)
#define RADIO_SYNC        "rikorsev"
#define RADIO_SYNC_SIZE   (8U)
#define RADIO_POWER_DB    (10)
#define RADIO_RSSI_THRESH (-80)

egl_iface_t *plat_radio_iface_get(void);

#endif