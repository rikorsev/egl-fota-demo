#ifndef RADIO_H
#define RADIO_H

typedef enum
{
    RADIO_TASK_RECV = 1,
    RADIO_TASK_LED_TOGGLE_SEND = 2,
}radio_task_t;

egl_result_t radio_task_set(radio_task_t task);
egl_result_t radio_task(void);

#endif