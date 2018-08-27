# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(basics_xx_memory ${SRC})

  target_link_libraries(basics_xx_memory
    ${LIBS}
    tubex-core)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME basics_xx_memory COMMAND basics_xx_memory 0)
  #endif(BUILD_TESTS)