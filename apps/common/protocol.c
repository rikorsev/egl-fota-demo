#include <string.h>

#include "protocol.h"
#include "plat.h"

static egl_result_t protocol_packet_crc_calc(protocol_packet_t *packet)
{
    egl_result_t result;

    result = egl_crc_reset(PLAT_CRC);
    EGL_RESULT_CHECK(result);

    uint32_t crc = egl_crc32_calc(PLAT_CRC, packet, sizeof(protocol_packet_t) + packet->len);

    memcpy((uint8_t *)packet + sizeof(protocol_packet_t) + packet->len, &crc, sizeof(crc));

    return EGL_SUCCESS;
}

egl_result_t protocol_packet_build(protocol_packet_t *packet, uint32_t cmd, uint8_t *payload, size_t len)
{
    egl_result_t result;

    packet->cmd = cmd;
    packet->len = len;

    if(payload != NULL)
    {
        memcpy(packet->payload, payload, len);
    }

    result = protocol_packet_crc_calc(packet);
    EGL_RESULT_CHECK(result);

    return EGL_SUCCESS;
}

static egl_result_t protocol_packet_crc_validate(protocol_packet_t *packet)
{
    egl_result_t result;
    uint32_t expected;
    uint8_t *expected_ptr = (uint8_t *)packet + sizeof(protocol_packet_t) + packet->len;

    memcpy(&expected, expected_ptr, sizeof(expected));

    result = egl_crc_reset(PLAT_CRC);
    EGL_RESULT_CHECK(result);

    uint32_t calculated = egl_crc32_calc(PLAT_CRC, packet, sizeof(protocol_packet_t) + packet->len);

    return calculated == expected ? EGL_SUCCESS : EGL_CHECKSUM_MISMATCH;
}

egl_result_t protocol_packet_validate(protocol_packet_t *packet, uint8_t exp_cmd, uint16_t exp_len)
{
    EGL_ASSERT_CHECK(packet->cmd == exp_cmd, EGL_FAIL);
    EGL_ASSERT_CHECK(packet->len == exp_len, EGL_FAIL);

    return protocol_packet_crc_validate(packet);
}