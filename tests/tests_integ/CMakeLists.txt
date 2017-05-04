# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


set(TESTS_INTEG_SOURCES
  tests_integ.cpp
)

add_executable(tubelib_tests_integ ${TESTS_INTEG_SOURCES})
target_link_libraries(tubelib_tests_integ tubex-core)
add_test(NAME TestInteg COMMAND tubelib_tests_integ)