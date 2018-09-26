# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(solver_05_delay ${SRC})

  target_link_libraries(solver_05_delay
    ${LIBS}
    tubex-core
    tubex-solver)

# Tests

  if(BUILD_TESTS)
    add_test(NAME solver_05_delay COMMAND solver_05_delay 0)
  endif(BUILD_TESTS)