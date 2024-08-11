/**
 * @file The module conatins list of avaliable EGL results and macroses to check a results of 
 *       different actions
 */

#ifndef EGL_RESULT_H
#define EGL_RESULT_H

#include <assert.h>

/** @brief list of EGL results */
typedef enum
{
    EGL_SUCCESS = 0,       /// Operation is successful
    EGL_RESET = 0,         /// The instance in reset state
    EGL_SET,               /// The instance in set state
    EGL_UNKNOWN,           /// Result of operation is unknown
    EGL_FAIL,              /// Operation has failed
    EGL_PROCESS,           /// Operation still in process
    EGL_NOT_SUPPORTED,     /// Operation is not supported
    EGL_OUT_OF_BOUNDARY,   /// Operation calue is out of boundary
    EGL_INVALID_PARAM,     /// Passed paramiter is invalid
    EGL_INVALID_STATE,     /// The instance is in invalid state
    EGL_ASSERT_FAIL,       /// Assertion fail
    EGL_TIMEOUT,           /// Operation timeout

    EGL_LAST
}egl_result_t;

#define EGL_RESULT(x) egl_result_str_get((x))
#define EGL_RESULT_FATAL()  assert(0);

#if CONFIG_EGL_RESULT_CHECK_ENABLED
#define EGL_ASSERT_CHECK(x) if(!(x)) { return EGL_ASSERT_FAIL; }
#define EGL_RESULT_CHECK(x) if((x) != EGL_SUCCESS) { return result; }
#else
#define EGL_ASSERT_CHECK(x)
#define EGL_RESULT_CHECK(x)
#endif

/**
 * @brief Convert egl result value to string
 */
char *egl_result_str_get(egl_result_t result);
#endif
