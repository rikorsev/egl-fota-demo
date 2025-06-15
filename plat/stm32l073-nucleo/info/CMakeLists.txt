configure_file(${CMAKE_CURRENT_LIST_DIR}/plat_info.c.in ${CMAKE_BINARY_DIR}/plat_info.c @ONLY)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_BINARY_DIR}/plat_info.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/plat_info.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})