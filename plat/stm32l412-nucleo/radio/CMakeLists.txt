target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_iface.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_nss_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_reset_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio0_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio2_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio4_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio5_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_tx_led_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_rx_led_pin.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_iface.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_nss_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_reset_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio0_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio2_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio4_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio5_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_tx_led_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_rx_led_pin.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})