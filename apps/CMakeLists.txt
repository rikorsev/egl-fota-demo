add_subdirectory(${TARGET_APP})

if(CONFIG_APP_APPEND_COMMON_PART)
    add_subdirectory(common)
endif()