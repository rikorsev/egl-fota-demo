#ifndef PLAT_RADIO_IFACE_H
#define PLAT_RADIO_IFACE_H

#define RADIO plat_radio_iface_get()

#define RADIO_FREQ      (915000000U)
#define RADIO_DEVIATION (30000U)
#define RADIO_BITRATE   (115200U)
#define RADIO_PREAMBLE  (16U)
#define RADIO_NODE_ADDR (162U)
#define RADIO_SYNC      "rikorsev"
#define RADIO_SYNC_SIZE (8U)
#define RADIO_POWER_DB  (13)

egl_interface_t *plat_radio_iface_get(void);

#endif