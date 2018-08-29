# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(rob_06_loops_detection ${SRC})

  target_link_libraries(rob_06_loops_detection
    ${LIBS}
    tubex-core
    tubex-robotics)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME rob_06_loops_detection COMMAND rob_06_loops_detection 0)
  #endif(BUILD_TESTS)