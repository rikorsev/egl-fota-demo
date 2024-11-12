# Get build time
execute_process(
    COMMAND bash -c "date"
    OUTPUT_VARIABLE PLAT_BUILD_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get git branch
execute_process(
    COMMAND bash -c "git rev-parse --abbrev-ref HEAD"
    OUTPUT_VARIABLE PLAT_GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get git hash
execute_process(
    COMMAND bash -c "git rev-parse HEAD"
    OUTPUT_VARIABLE PLAT_GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Create .bin
add_custom_target(${PROJECT_NAME}.${TARGET_SLOT}.bin ALL DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJCOPY} -v -O binary ${CMAKE_BINARY_DIR}/apps/${TARGET_APP}/${PROJECT_NAME}.${TARGET_SLOT}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${TARGET_SLOT}.bin)

# Fullfill the binary with meta
add_custom_target(${PROJECT_NAME}.${TARGET_SLOT}.bin.meta ALL DEPENDS ${PROJECT_NAME}
    COMMAND python3 ${CMAKE_CURRENT_LIST_DIR}/meta.py ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${TARGET_SLOT}.bin ${CONFIG_PLAT_SLOT_INFO_OFFSET} ${CONFIG_PLAT_SLOT_INFO_SECTION_SIZE})
