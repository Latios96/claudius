# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions( -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}" )

add_subdirectory(io)

# add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
#
# find_package(Maya REQUIRED)
#
# include_directories(${MAYA_INCLUDE_DIR})
# link_directories(${MAYA_LIBRARY_DIR})
# target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})
#
# MAYA_PLUGIN(${PROJECT_NAME})
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)
