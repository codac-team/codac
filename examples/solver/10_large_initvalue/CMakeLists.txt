# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(solver_10_large_initvalue ${SRC})

  target_link_libraries(solver_10_large_initvalue
    ${LIBS}
    tubex-core
    tubex-solver)

# Tests

  if(BUILD_TESTS)
    add_test(NAME solver_10_large_initvalue COMMAND solver_10_large_initvalue 0)
  endif(BUILD_TESTS)