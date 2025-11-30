target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/plat_crc.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/plat_crc.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})