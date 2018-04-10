set(TESTS_TUBE_SOURCES
  tests.cpp
  tests_definition.cpp
  tests_exceptions.cpp
  tests_operators.cpp
  tests_slicesstructure.cpp
  tests_values.cpp
)

add_executable(tubelib_tests_tube ${TESTS_TUBE_SOURCES})
target_link_libraries(tubelib_tests_tube tubex-lib)
add_test(NAME TestTube COMMAND tubelib_tests_tube)