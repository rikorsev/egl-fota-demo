target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio0.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio1.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio2.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio3.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio4.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_dio5.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_iface.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_clock.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm_pm.c
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/plat_rfm.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})