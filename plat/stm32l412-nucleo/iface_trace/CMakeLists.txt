target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/iface_trace.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/iface_trace.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})