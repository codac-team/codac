# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Sources

  set(SRC main.cpp)
  add_executable(rob_05_data_association ${SRC})

  # Link with ibex-geometry (used for CtcPolar)
  set(LIBS ${LIBS} -libex-geometry)
  link_directories(/usr/local/lib/ibex-geometry/)
  include_directories(/usr/local/include/ibex-geometry/)

  target_link_libraries(rob_05_data_association
    ${LIBS}
    tubex-core
    tubex-robotics)

# Tests

  if(BUILD_TESTS)
    add_test(NAME rob_05_data_association COMMAND rob_05_data_association 0)
  endif(BUILD_TESTS)