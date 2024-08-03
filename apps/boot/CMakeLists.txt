cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

project(afd_boot LANGUAGES C ASM)

add_executable(${PROJECT_NAME})
set(CMAKE_EXECUTABLE_SUFFIX ".${TARGET_SLOT}.elf")

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.c
)

include(app_config.cmake)
include(egl_config.cmake)
include(${ROOT_DIR}/plat/${TARGET_PLATFORM}/plat.cmake)

# Add EGL library
add_subdirectory(${ROOT_DIR}/egl ${CMAKE_BINARY_DIR}/egl_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE egl_lib)

# Add platform
add_subdirectory(${ROOT_DIR}/plat/${TARGET_PLATFORM} ${CMAKE_BINARY_DIR}/plat/${TARGET_PLATFORM})
