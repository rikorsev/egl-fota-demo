#ifndef FOTA_H
#define FOTA_H

#include "egl_lib.h"
#include "plat.h"

egl_result_t fota_init(void);
egl_result_t fota_start(void);
egl_result_t fota_request_chunk_handle(protocol_packet_t *packet);
egl_result_t fota_complete_handle(protocol_packet_t *packet);
egl_result_t fota_request_meta_handle(protocol_packet_t *packet);
egl_result_t fota_response_meta_handle(protocol_packet_t *packet);

#endif