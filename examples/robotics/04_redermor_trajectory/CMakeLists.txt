# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(rob_04_redermor_beacons ${SRC})

  target_link_libraries(rob_04_redermor_beacons
    ${LIBS}
    tubex-core
    tubex-robotics)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME rob_04_redermor_beacons COMMAND rob_04_redermor_beacons 0)
  #endif(BUILD_TESTS)

  # Double test for data serialization checking

  #if(BUILD_TESTS)
  #  add_test(NAME rob_04_redermor_beacons COMMAND rob_04_redermor_beacons 0)
  #endif(BUILD_TESTS)