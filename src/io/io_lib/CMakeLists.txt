
set(CLAUDIUS_IO_LIB_SOURCES
        Greeter.h
        Greeter.cpp
        )
add_library(claudios_io_lib STATIC
        ${CLAUDIUS_IO_LIB_SOURCES}
)

target_include_directories(claudios_io_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )