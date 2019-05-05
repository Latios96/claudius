# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND git describe --tags
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions( -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}" )

add_subdirectory(utils)
add_subdirectory(io)
add_subdirectory(plugin)
add_subdirectory(procedural)

