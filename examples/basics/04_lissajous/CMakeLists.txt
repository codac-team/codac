# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(basics_04_lissajous ${SRC})

  target_link_libraries(basics_04_lissajous
    ${LIBS}
    tubex-core)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME basics_04_lissajous COMMAND basics_04_lissajous 0)
  #endif(BUILD_TESTS)