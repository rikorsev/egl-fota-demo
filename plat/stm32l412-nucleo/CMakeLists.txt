target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/plat.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis/Device/ST/STM32L4xx/Source/Templates/gcc/startup_stm32l412xx.s
        ${CMAKE_CURRENT_LIST_DIR}/cmsis/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/plat.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cmsis/Device/ST/STM32L4xx/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cmsis/Core/Include)

add_subdirectory(pio_led_green)
