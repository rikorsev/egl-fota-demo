#ifndef SWITCH_H
#define SWITCH_H

enum
{
    SWITCH_TOGGLE_RECV_FLAG = 1,
    SWITCH_TOGGLE_SEND_FLAG = 2
};

egl_result_t switch_init(void);
egl_result_t switch_request(void);
egl_result_t switch_request_handle(protocol_packet_t *packet);

#endif