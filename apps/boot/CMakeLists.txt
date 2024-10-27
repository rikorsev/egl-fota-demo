add_executable(${PROJECT_NAME})
set(CMAKE_EXECUTABLE_SUFFIX ".${TARGET_SLOT}.elf")

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/fota_mgr.c
        ${CMAKE_CURRENT_LIST_DIR}/boot_mgr.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/fota_mgr.h
        ${CMAKE_CURRENT_LIST_DIR}/boot_mgr.h
)

add_subdirectory(radio)

target_link_libraries(${PROJECT_NAME} PRIVATE m)
