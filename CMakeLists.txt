cmake_minimum_required(VERSION 3.22.2 FATAL_ERROR)

project(${TARGET_APP} LANGUAGES C ASM)

include(apps/${TARGET_APP}/app_config.cmake)
include(apps/${TARGET_APP}/egl_config.cmake)
include(plat/${TARGET_PLATFORM}/plat_config.cmake)

# Add an application
add_subdirectory(apps)

# Add platform
add_subdirectory(plat/${TARGET_PLATFORM})

# Add EGL library
add_subdirectory(egl)
target_link_libraries(${PROJECT_NAME} PRIVATE egl_lib)