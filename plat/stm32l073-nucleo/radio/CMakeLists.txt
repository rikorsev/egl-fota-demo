add_subdirectory(rfm)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/plat_radio_iface_${TARGET_RFM}.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_radio_tx_led.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_radio_rx_led.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/plat_radio_iface.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_radio_tx_led.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_radio_rx_led.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})