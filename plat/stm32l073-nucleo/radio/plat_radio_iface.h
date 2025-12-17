#ifndef PLAT_RADIO_IFACE_H
#define PLAT_RADIO_IFACE_H

#include "plat_rfm.h"
#include "plat_radio_sw1.h"
#include "plat_radio_sw2.h"

#define RADIO plat_radio_iface_get()

#define RADIO_FREQ                  (915000000U)
#define RADIO_DEVIATION             (35000)
#define RADIO_BITRATE               (115200)
#define RADIO_PREAMBLE              (16U)
#define RADIO_NODE_ADDR             (1U)
#define RADIO_SYNC                  "kolosov"
#define RADIO_SYNC_SIZE             (8U)
#define RADIO_POWER_DB              (10)
#define RADIO_RSSI_THRESH           (-80)
#define RADIO_TX_INTER_PACKET_DELAY (5U)

enum
{
    RADIO_IOCTL_RTS_CALLBACK_SET = 1,
    RADIO_IOCTL_RX_MODE_SET,
    RADIO_IOCTL_RX_TIMEOUT_SET
};

egl_iface_t *plat_radio_iface_get(void);

#endif