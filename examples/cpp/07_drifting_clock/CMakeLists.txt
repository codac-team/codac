# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_drifting_clock ${SRC})

  target_link_libraries(ex_drifting_clock
    ${LIBS}
    tubex-lib)


# Tests

  if(BUILD_TESTS)
    add_test(NAME ExampleDriftingClock COMMAND ex_drifting_clock 0)
  endif(BUILD_TESTS)