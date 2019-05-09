set(LIBRARY_NAME ${PROJECT_NAME}MtoaExtension)

set(LIB_SRCS
        extension.cpp
        Translator.h
        Translator.cpp
        )

find_package(MtoA REQUIRED)

add_library(${LIBRARY_NAME} SHARED ${LIB_SRCS})

target_include_directories(${LIBRARY_NAME} PUBLIC ${ARNOLD_INCLUDE_DIR} ${MTOA_INCLUDE_DIR} ${MAYA_INCLUDE_DIR})
target_link_directories(${LIBRARY_NAME} PUBLIC ${ARNOLD_LIBRARY_DIR} ${MAYA_LIBRARY_DIR} C:/solidangle/mtoadeploy/2018/lib C:/solidangle/mtoadeploy/2018/bin)

# NOTE: no ${MAYA_LIBRARIES}, this does not work for some reason
target_link_libraries(${LIBRARY_NAME} ai.lib OpenGl32.lib glu32.lib Foundation.lib OpenMaya.lib OpenMayaRender.lib OpenMayaUI.lib OpenMayaAnim.lib OpenMayaFX.lib mtoa_api.lib)