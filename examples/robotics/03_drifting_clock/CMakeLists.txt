# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(rob_03_drifting_clock ${SRC})

  target_link_libraries(rob_03_drifting_clock
    ${LIBS}
    tubex-core
    tubex-robotics)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME rob_03_drifting_clock COMMAND rob_03_drifting_clock 0)
  #endif(BUILD_TESTS)