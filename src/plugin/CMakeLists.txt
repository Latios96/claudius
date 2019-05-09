set(SOURCE_FILES
        plugin.cpp
        ClaudiusVisualizer.h
        ClaudiusVisualizer.cpp
        ClaudiusVisualizerDrawOverride.h
        ClaudiusVisualizerDrawOverride.cpp
        PointCloudFileTranslator.h
        PointCloudFileTranslator.cpp
        )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
# make sure this is build after arnold procedural, so we can copy it in the right directory
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}ArnoldProcedural ${PROJECT_NAME}MtoaExtension)

find_package(Maya REQUIRED)

include_directories(${MAYA_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES} opengl32 claudius_utils_lib claudius_io_lib)

MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# configure launch maya bat
set(LAUNCH_MAYA_BAT launch_maya_with_claudius.bat)
configure_file(launch_maya_with_claudius_template.bat ${LAUNCH_MAYA_BAT})

set(CLAUDIUS_MAYA_MODULE claudius.mod)
configure_file(claudius_template.mod ${CLAUDIUS_MAYA_MODULE})


# installation as Maya Module
# todo use version number
set(PROJECT_PLAT)
set(MAYA_MODULE_NAME claudius-${GIT_COMMIT_HASH}-Maya-${MAYA_VERSION}-${CMAKE_SYSTEM_NAME})

set(MAYA_MODULE_FOLDER $<TARGET_FILE_DIR:${PROJECT_NAME}>/${MAYA_MODULE_NAME})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/module ${MAYA_MODULE_FOLDER})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove ${MAYA_MODULE_FOLDER}/plug-ins/.gitkeep)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove ${MAYA_MODULE_FOLDER}/procedurals/.gitkeep)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove ${MAYA_MODULE_FOLDER}/extensions/.gitkeep)

# copy maya plugin
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.mll ${MAYA_MODULE_FOLDER}/plug-ins/${PROJECT_NAME}.mll)

# copy maya wrapper batch file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}>/../${LAUNCH_MAYA_BAT} ${MAYA_MODULE_FOLDER}/${LAUNCH_MAYA_BAT})
# copy maya module
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}>/../${CLAUDIUS_MAYA_MODULE} ${MAYA_MODULE_FOLDER}/${CLAUDIUS_MAYA_MODULE})

# copy arnold procedural
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}ArnoldProcedural>/${PROJECT_NAME}ArnoldProcedural.dll ${MAYA_MODULE_FOLDER}/procedurals/${PROJECT_NAME}ArnoldProcedural.dll)

# copy arnold extension
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}MtoaExtension>/${PROJECT_NAME}MtoaExtension.dll ${MAYA_MODULE_FOLDER}/extensions/${PROJECT_NAME}MtoaExtension.dll)

# zip it
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "C:/Program Files/7-Zip/7z.exe" a -tzip ${MAYA_MODULE_FOLDER}.zip ${MAYA_MODULE_FOLDER})
