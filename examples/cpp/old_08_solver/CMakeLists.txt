# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_solver ${SRC})

  target_link_libraries(ex_solver
    ${LIBS}
    tubex-lib)


# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME ExampleSolver COMMAND ex_solver 0)
  #endif(BUILD_TESTS)