
set(CLAUDIUS_IO_LIB_SOURCES
        Greeter.h
        Greeter.cpp
        ParticleContainer.h
        ParticleContainer.cpp
        ParticleReader.h
        ParticleReaderFactory.h
        ParticleReaderFactory.cpp
        )
add_library(claudius_io_lib STATIC
        ${CLAUDIUS_IO_LIB_SOURCES}
)

target_include_directories(claudius_io_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )