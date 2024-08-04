target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/radio.c
        ${CMAKE_CURRENT_LIST_DIR}/sx1232-Hal.c
        ${CMAKE_CURRENT_LIST_DIR}/sx1232-Misc.c
        ${CMAKE_CURRENT_LIST_DIR}/sx1232.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/radio.h
        ${CMAKE_CURRENT_LIST_DIR}/sx1232-Hal.h
        ${CMAKE_CURRENT_LIST_DIR}/sx1232-Misc.h
        ${CMAKE_CURRENT_LIST_DIR}/sx1232.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})