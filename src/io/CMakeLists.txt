add_subdirectory(io_lib)

set(CLAUDIUS_IO_CLI_SOURCES
        main.cpp
        )

add_executable(claudius_cli ${CLAUDIUS_IO_CLI_SOURCES} )



target_link_libraries(claudius_cli
        claudius_io_lib claudius_utils_lib
        )

install(TARGETS claudius_cli DESTINATION bin)
