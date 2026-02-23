#ifndef SWITCH_H
#define SWITCH_H

enum
{
    SWITCH_TOGGLE_RECV_FLAG = 1,
    SWITCH_TOGGLE_SEND_FLAG = 2
};

egl_result_t switch_flag_set(unsigned int flag);
egl_result_t switch_toggle_recv_cmd_handle(protocol_packet_t *packet);
egl_result_t switch_init(void);

#endif