set(SOURCE_FILES
        plugin.cpp
        ClaudiusVisualizer.h
        ClaudiusVisualizer.cpp
        ClaudiusVisualizerDrawOverride.h
        ClaudiusVisualizerDrawOverride.cpp
        )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

find_package(Maya REQUIRED)

include_directories(${MAYA_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES} opengl32 claudius_utils_lib)

MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
