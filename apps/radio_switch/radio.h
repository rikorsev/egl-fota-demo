#ifndef RADIO_H
#define RADIO_H

enum
{
    RADIO_RECV_FLAG = 1,
    RADIO_LED_TOGGLE_SEND_FLAG = 2,
};

egl_result_t radio_flag_set(unsigned int flag);
egl_result_t radio_init(void);

#endif