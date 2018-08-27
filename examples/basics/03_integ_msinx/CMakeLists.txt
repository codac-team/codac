# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(basics_03_ivp_msinx ${SRC})

  target_link_libraries(basics_03_ivp_msinx
    ${LIBS}
    tubex-core)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME basics_03_ivp_msinx COMMAND basics_03_ivp_msinx 0)
  #endif(BUILD_TESTS)