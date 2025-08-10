target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/plat_usrbtn.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/plat_usrbtn.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})