# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_arithmetic ${SRC})

  target_link_libraries(ex_arithmetic
    ${LIBS}
    tubex-core)


# Tests

  if(BUILD_TESTS)
    add_test(NAME ExampleArithmetics COMMAND ex_arithmetic 0)
  endif(BUILD_TESTS)