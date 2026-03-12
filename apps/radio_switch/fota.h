#ifndef FOTA_H
#define FOTA_H

#include "egl_lib.h"
#include "plat.h"

egl_result_t fota_init(void);
egl_result_t fota_start(void);
egl_result_t fota_request_handle(protocol_packet_t *packet);

#endif