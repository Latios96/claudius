project( claudius ) # accessable with ${PROJECT_NAME}
cmake_minimum_required( VERSION 3.1 )

# Activate C++ 14 features, so we are compliant with the VFX Reference Platform
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# load out FindMaya module
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)

add_subdirectory(src)
add_subdirectory(test)
