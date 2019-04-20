
set(CLAUDIUS_UTILS_LIB_SOURCES
        version.h
        version.cpp
        )
add_library(claudius_utils_lib STATIC
        ${CLAUDIUS_UTILS_LIB_SOURCES}
        )

target_include_directories(claudius_utils_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )