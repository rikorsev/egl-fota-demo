#ifndef SWITCH_H
#define SWITCH_H

typedef enum
{
    SWITCH_TASK_LOCAL_STATE_TOGGLE = 2
}switch_task_t;

egl_result_t switch_task_set(switch_task_t task);
egl_result_t switch_task(void);

#endif