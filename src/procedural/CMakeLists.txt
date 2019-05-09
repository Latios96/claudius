set(POINT_INSTANCER_SOURCES claudius_particles.cpp)

find_package(Arnold REQUIRED)
# todo rename to ClaudiusArnoldProcedural
# todo declare all these names globally
set(LIBRARY_NAME ${PROJECT_NAME}ArnoldProcedural)
add_library(${LIBRARY_NAME} SHARED ${POINT_INSTANCER_SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${ARNOLD_INCLUDE_DIR})
target_link_directories(${LIBRARY_NAME} PUBLIC ${ARNOLD_LIBRARY_DIR})

target_link_libraries(${LIBRARY_NAME} ai claudius_io_lib)