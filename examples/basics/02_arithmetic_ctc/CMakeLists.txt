# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(basics_02_arithmetic_ctc ${SRC})

  target_link_libraries(basics_02_arithmetic_ctc
    ${LIBS}
    tubex-core)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME basics_02_arithmetic_ctc COMMAND basics_02_arithmetic_ctc 0)
  #endif(BUILD_TESTS)