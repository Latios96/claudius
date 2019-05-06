set(POINT_INSTANCER_SOURCES claudius_particles.cpp)

find_package(Arnold REQUIRED)

add_library(MyPointInstancer SHARED ${POINT_INSTANCER_SOURCES})

target_include_directories(MyPointInstancer PUBLIC ${ARNOLD_INCLUDE_DIR})
target_link_directories(MyPointInstancer PUBLIC ${ARNOLD_LIBRARY_DIR})

target_link_libraries(MyPointInstancer ai claudius_io_lib)