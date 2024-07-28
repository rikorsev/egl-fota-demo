target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pio_led_green.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/pio_led_green.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})