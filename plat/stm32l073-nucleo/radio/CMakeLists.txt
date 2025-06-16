add_subdirectory(rfm)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/plat_radio_iface.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/plat_radio_iface.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})