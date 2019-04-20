set(TARGET_NAME tests)

add_executable(${TARGET_NAME}
  main.cpp
    testParticleContainer.cpp testParticleReaderFactory.cpp testPtsParticleReader.cpp)

target_link_libraries(${TARGET_NAME}
  PUBLIC claudius_io_lib)

target_include_directories(${TARGET_NAME}
  PUBLIC ../externals/catch2/ ../externals/fakeIt)

add_test(
  NAME ${TARGET_NAME}
  COMMAND ${TARGET_NAME} -o report.xml -r junit)