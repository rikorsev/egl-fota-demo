#ifndef SWITCH_H
#define SWITCH_H

enum
{
    SWITCH_TOGGLE_FLAG = 1
};

egl_result_t switch_flag_set(unsigned int flag);
egl_result_t switch_init(void);

#endif