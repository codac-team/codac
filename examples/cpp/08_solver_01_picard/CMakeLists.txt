# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(solver_01_picard ${SRC})

  target_link_libraries(solver_01_picard
    ${LIBS}
    tubex-lib)


# Tests

  if(BUILD_TESTS)
    add_test(NAME solver_01_picard COMMAND solver_01_picard 0)
  endif(BUILD_TESTS)