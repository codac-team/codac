# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(solver_02_xmsin_fwd ${SRC})

  target_link_libraries(solver_02_xmsin_fwd
    ${LIBS}
    tubex-core
    tubex-solver)

# Tests

  if(BUILD_TESTS)
    add_test(NAME solver_02_xmsin_fwd COMMAND solver_02_xmsin_fwd 0)
  endif(BUILD_TESTS)