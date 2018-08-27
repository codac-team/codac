# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(basics_01_arithmetic ${SRC})

  target_link_libraries(basics_01_arithmetic
    ${LIBS}
    tubex-core)

# Tests

  if(BUILD_TESTS)
    add_test(NAME basics_01_arithmetic COMMAND basics_01_arithmetic 0)
  endif(BUILD_TESTS)