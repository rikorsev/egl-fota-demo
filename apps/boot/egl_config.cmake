
if(CMAKE_BUILD_TYPE STREQUAL "release")
    set(CONFIG_EGL_TRACE_ENABLED 1)
    set(CONFIG_EGL_RESULT_CHECK_ENABLED 0)
else()
    set(CONFIG_EGL_TRACE_ENABLED 1)
    set(CONFIG_EGL_RESULT_CHECK_ENABLED 1)
endif()

if(CONFIG_EGL_TRACE_ENABLED EQUAL 1)
    add_definitions(-DCONFIG_EGL_TRACE_ENABLED=1)
    add_definitions(-DCONFIG_EGL_TRACE_BUFF_SIZE=256)
else()
    add_definitions(-DCONFIG_EGL_TRACE_ENABLED=0)
    add_definitions(-DCONFIG_EGL_TRACE_BUFF_SIZE=0)
endif()

if(CONFIG_EGL_RESULT_CHECK_ENABLED EQUAL 1)
    add_definitions(-DCONFIG_EGL_RESULT_CHECK_ENABLED=1)
else()
    add_definitions(-DCONFIG_EGL_RESULT_CHECK_ENABLED=0)
endif()
