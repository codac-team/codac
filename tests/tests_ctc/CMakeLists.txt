# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


set(TESTS_CTC_SOURCES
  tests_ctc.cpp
)

add_executable(tubelib_tests_ctc ${TESTS_CTC_SOURCES})
target_link_libraries(tubelib_tests_ctc tubex-lib)
add_test(NAME TestCtc COMMAND tubelib_tests_ctc)