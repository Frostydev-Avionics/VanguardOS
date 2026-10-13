if(CONFIG_ETC_ROMFS)
    # Symlink our etc directory into the build directory so nuttx_add_romfs
    # can find it when building the ROMFS image
    execute_process(
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_CURRENT_SOURCE_DIR}/init
            ${CMAKE_BINARY_DIR}/etc
    )
endif()

add_subdirectory(src)
