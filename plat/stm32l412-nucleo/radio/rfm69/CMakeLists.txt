target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio0_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio0_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio1_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio2_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio3_pin.c
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio4_pin.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio0_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio0_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio1_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio2_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio3_pin.h
        ${CMAKE_CURRENT_LIST_DIR}/palt_rfm_dio4_pin.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})