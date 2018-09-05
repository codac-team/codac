# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(solver_03_xmsin_bwd ${SRC})

  target_link_libraries(solver_03_xmsin_bwd
    ${LIBS}
    tubex-core
    tubex-solver)

# Tests

  if(BUILD_TESTS)
    add_test(NAME solver_03_xmsin_bwd COMMAND solver_03_xmsin_bwd 0)
  endif(BUILD_TESTS)