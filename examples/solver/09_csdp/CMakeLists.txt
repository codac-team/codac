# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(solver_09_csdp ${SRC})

  target_link_libraries(solver_09_csdp
    ${LIBS}
    tubex-core
    tubex-solver)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME solver_09_csdp COMMAND solver_09_csdp 0)
  #endif(BUILD_TESTS)