# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


set(TESTS_BASE_SOURCES
  tests_base.cpp
)

add_executable(tubelib_tests_base ${TESTS_BASE_SOURCES})
target_link_libraries(tubelib_tests_base tubex-core)
add_test(NAME TestBase COMMAND tubelib_tests_base)