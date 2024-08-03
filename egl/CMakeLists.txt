cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

project(egl_lib LANGUAGES C)

add_library(${PROJECT_NAME})

add_subdirectory(interface)
add_subdirectory(pio)
add_subdirectory(plat)
add_subdirectory(result)
add_subdirectory(timer)
add_subdirectory(trace)
add_subdirectory(crc)
add_subdirectory(block)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
