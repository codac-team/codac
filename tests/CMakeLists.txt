set(TESTS_SOURCES
  main.cpp
  tests_definition.cpp
  tests_exceptions.cpp
  tests_operators.cpp
  tests_serialization.cpp
  tests_slicesstructure.cpp
  tests_trajectory.cpp
  tests_values.cpp
)

add_executable(tubelib_tests ${TESTS_SOURCES})
target_link_libraries(tubelib_tests tubex-lib)
add_test(NAME Test COMMAND tubelib_tests)