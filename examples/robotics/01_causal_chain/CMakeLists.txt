# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(rob_01_causal_chain ${SRC})

  target_link_libraries(rob_01_causal_chain
    ${LIBS}
    tubex-core
    tubex-robotics)

# Tests

  if(BUILD_TESTS)
    add_test(NAME rob_01_causal_chain COMMAND rob_01_causal_chain 0)
  endif(BUILD_TESTS)