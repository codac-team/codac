# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_integ-msinx ${SRC})

  target_link_libraries(ex_integ-msinx
    ${LIBS}
    tube-core
    tube-graphics)


# Tests

  if(BUILD_TESTS)
    add_test(ex_integ-msinx ex_integ-msinx 0)
  endif(BUILD_TESTS)