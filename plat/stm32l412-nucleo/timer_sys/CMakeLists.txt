target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/timer_sys.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/timer_sys.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})