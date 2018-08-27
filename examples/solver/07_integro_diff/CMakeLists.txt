# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(solver_07_integro_diff ${SRC})

  target_link_libraries(solver_07_integro_diff
    ${LIBS}
    tubex-core
    tubex-solver)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME solver_07_integro_diff COMMAND solver_07_integro_diff 0)
  #endif(BUILD_TESTS)