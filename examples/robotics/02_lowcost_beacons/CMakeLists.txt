# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(rob_02_lowcost_beacons ${SRC})

  target_link_libraries(rob_02_lowcost_beacons
    ${LIBS}
    tubex-core
    tubex-robotics)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME rob_02_lowcost_beacons COMMAND rob_02_lowcost_beacons 0)
  #endif(BUILD_TESTS)