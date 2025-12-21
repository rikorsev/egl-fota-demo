add_executable(${PROJECT_NAME})
set(CMAKE_EXECUTABLE_SUFFIX ".${CONFIG_APP_PROJECT_SUFFIX}.elf")

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/boot_mgr.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/boot_mgr.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE m)
