include(${CMAKE_CURRENT_LIST_DIR}/flags.cmake)

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

configure_file(${CMAKE_CURRENT_LIST_DIR}/plat_info.c.in ${CMAKE_BINARY_DIR}/plat_info.c @ONLY)