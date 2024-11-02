target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-device-l0/Source/Templates/gcc/startup_stm32l073xx.s
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-device-l0/Source/Templates/system_stm32l0xx.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cmsis-device-l0/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cmsis-arm/CMSIS/Core/Include)

add_subdirectory(plat)
add_subdirectory(slot)
add_subdirectory(sysled)
add_subdirectory(systimer)
add_subdirectory(syslog)