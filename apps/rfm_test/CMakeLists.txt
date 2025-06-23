add_executable(${PROJECT_NAME})
set(CMAKE_EXECUTABLE_SUFFIX ".${TARGET_SLOT}.elf")

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/rfm_test.h
)

if(${TARGET_RFM} STREQUAL "rfm69")
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/rfm69_test.c
    )
elseif(${TARGET_RFM} STREQUAL "rfm66")
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/rfm66_test.c
    )
endif()
