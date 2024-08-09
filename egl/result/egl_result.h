#ifndef EGL_RESULT_H
#define EGL_RESULT_H

#include <assert.h>

typedef enum
{
    EGL_SUCCESS,
    EGL_UNKNOWN,
    EGL_FAIL,
    EGL_SET,
    EGL_RESET,
    EGL_PROCESS,
    EGL_NOT_SUPPORTED,
    EGL_OUT_OF_BOUNDARY,
    EGL_INVALID_PARAM,
    EGL_INVALID_STATE,
    EGL_ASSERT_FAIL,
    EGL_TIMEOUT,

    EGL_LAST
}egl_result_t;

#define EGL_RESULT(x) egl_result_str_get((x))
#define EGL_ASSERT_CHECK(x) if(!(x)) { return EGL_ASSERT_FAIL; }
#define EGL_RESULT_CHECK(x) if((x) != EGL_SUCCESS) { return result; }
#define EGL_RESULT_FATAL()  assert(0);

char *egl_result_str_get(egl_result_t result);
#endif
