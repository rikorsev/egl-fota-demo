configure_file(${CMAKE_CURRENT_LIST_DIR}/slot.c.in ${CMAKE_BINARY_DIR}/slot.c @ONLY)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_BINARY_DIR}/slot.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/slot.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})