#ifndef RADIO_H
#define RADIO_H

#include "egl_lib.h"
#include "protocol.h"

typedef egl_result_t (*radio_packet_recv_handler_func_t)(protocol_packet_t *packet);

egl_result_t radio_init(radio_packet_recv_handler_func_t handler);
egl_result_t radio_packet_send(protocol_packet_t *packet);


#endif