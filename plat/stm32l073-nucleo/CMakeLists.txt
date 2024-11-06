target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/startup_stm32l073xx.s
        ${CMAKE_CURRENT_LIST_DIR}/isr.c
)

if(CONFIG_APP_MINIMAL EQUAL 1)
    # Add custom minimal libc implementation
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib.s)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cmsis-device-l0/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/cmsis-arm/CMSIS/Core/Include)

add_subdirectory(plat)
add_subdirectory(slot)
add_subdirectory(sysled)
add_subdirectory(systimer)
add_subdirectory(syslog)
add_subdirectory(sysclock)