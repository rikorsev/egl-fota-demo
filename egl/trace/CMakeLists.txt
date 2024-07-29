target_sources(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/egl_trace.h
)

# include(${CMAKE_SOURCE_DIR}/egl_config.cmake)

# message("!!! TRACE State: ${CONFIG_EGL_TRACE_ENABLED}")

if(DEFINED CONFIG_EGL_TRACE_ENABLED AND CONFIG_EGL_TRACE_ENABLED EQUAL 1)
    target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/egl_trace.c
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})