
set(CLAUDIUS_IO_LIB_SOURCES
        ParticleContainer.h
        ParticleContainer.cpp
        ParticleReader.h
        ParticleReaderFactory.h
        ParticleReaderFactory.cpp
        PtsParticleReader.h
        PtsParticleReader.cpp
        ProgressReporter.h
        ProgressReporter.cpp
        RemainingTimeCalculator.h
        RemainingTimeCalculator.cpp
        )
add_library(claudius_io_lib STATIC
        ${CLAUDIUS_IO_LIB_SOURCES}
)

add_definitions(-DFMT_HEADER_ONLY)

target_include_directories(claudius_io_lib
        PUBLIC ../../../externals/fmt/)

target_include_directories(claudius_io_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )