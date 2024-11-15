target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cmsis/Device/ST/STM32L4xx/Source/Templates/gcc/startup_stm32l412xx.s
        ${CMAKE_CURRENT_LIST_DIR}/cmsis/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c
        ${CMAKE_CURRENT_LIST_DIR}/isr.c
)

# Configure linker script
configure_file(${CMAKE_CURRENT_LIST_DIR}/linker/stm32l412rbtx.ld.in ${CMAKE_BINARY_DIR}/stm32l412rbtx.ld @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cmsis/Device/ST/STM32L4xx/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cmsis/Core/Include)

add_subdirectory(plat)
add_subdirectory(slot)
add_subdirectory(sysled)
add_subdirectory(syslog)
add_subdirectory(systimer)
add_subdirectory(crc)
add_subdirectory(flash)
add_subdirectory(radio)
add_subdirectory(sysclock)
add_subdirectory(syspm)
