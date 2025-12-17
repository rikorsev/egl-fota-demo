#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "egl_lib.h"

enum
{
    PROTOCOL_CMD_ACK = 1,
    PROTOCOL_CMD_NAK,
    PROTOCOL_CMD_FOTA_REQUEST_TO_DOWNLOAD,
    PROTOCOL_CMD_FOTA_REQUEST_PAGE,
    PROTOCOL_CMD_FOTA_SEND_PAGE,
    PROTOCOL_CMD_FOTA_COMPLETE,
    PROTOCOL_CMD_SWITCH,
};

typedef struct
{
    uint8_t cmd;
    uint16_t len;
    uint8_t payload[];
}protocol_packet_t;

#define PROTOCOL_PACKET_DECLARE(name, payload) \
uint8_t name##_buff[sizeof(protocol_packet_t) + (payload) + sizeof(uint32_t)]; \
protocol_packet_t *name = (protocol_packet_t *)name##_buff

egl_result_t protocol_packet_build(protocol_packet_t *packet, uint32_t cmd, uint8_t *payload, size_t len);
egl_result_t protocol_packet_validate(protocol_packet_t *packet, uint8_t exp_cmd, uint16_t exp_len);

#endif