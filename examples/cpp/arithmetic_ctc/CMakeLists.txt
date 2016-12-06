# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_arithmetic_ctc ${SRC})

  target_link_libraries(ex_arithmetic_ctc
    ${LIBS}
    tube-core
    tube-graphics)


# Tests

  if(BUILD_TESTS)
    add_test(NAME ExampleArithmetics COMMAND ex_arithmetic_ctc 0)
  endif(BUILD_TESTS)