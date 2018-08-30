# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(solver_10_large_initial_condition ${SRC})

  target_link_libraries(solver_10_large_initial_condition
    ${LIBS}
    tubex-core
    tubex-solver)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME solver_10_large_initial_condition COMMAND solver_10_large_initial_condition 0)
  #endif(BUILD_TESTS)