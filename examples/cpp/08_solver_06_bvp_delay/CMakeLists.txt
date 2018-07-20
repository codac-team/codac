# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(solver_06_bvp_delay ${SRC})

  target_link_libraries(solver_06_bvp_delay
    ${LIBS}
    tubex-core
    tubex-solver)


# Tests

  if(BUILD_TESTS)
    add_test(NAME solver_06_bvp_delay COMMAND solver_06_bvp_delay 0)
  endif(BUILD_TESTS)