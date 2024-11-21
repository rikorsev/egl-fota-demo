add_executable(${PROJECT_NAME})
set(CMAKE_EXECUTABLE_SUFFIX ".${TARGET_SLOT}.elf")

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/rfm_test.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/rfm_test.h
)
