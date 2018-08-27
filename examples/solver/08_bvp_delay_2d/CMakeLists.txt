# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(solver_09_bvp_delay_2d ${SRC})

  target_link_libraries(solver_09_bvp_delay_2d
    ${LIBS}
    tubex-core
    tubex-solver)


# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME solver_09_bvp_delay_2d COMMAND solver_09_bvp_delay_2d 0)
  #endif(BUILD_TESTS)