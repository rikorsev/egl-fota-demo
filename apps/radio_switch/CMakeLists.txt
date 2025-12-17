add_executable(${PROJECT_NAME})

set(CMAKE_EXECUTABLE_SUFFIX ".${CONFIG_APP_PROJECT_SUFFIX}.elf")

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.c
)
