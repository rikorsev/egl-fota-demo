cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

project(afs_main LANGUAGES C ASM)

add_executable(${PROJECT_NAME})
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.c
)

include(egl_config.cmake)
include(${ROOT_DIR}/plat/${TARGET_PLATFORM}/flags.cmake)

# Add EGL library
add_subdirectory(${ROOT_DIR}/egl ${CMAKE_BINARY_DIR}/egl_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE egl_lib)

# Add platform
add_subdirectory(${ROOT_DIR}/plat/${TARGET_PLATFORM} ${CMAKE_BINARY_DIR}/plat/${TARGET_PLATFORM})

# Create .bin
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)
