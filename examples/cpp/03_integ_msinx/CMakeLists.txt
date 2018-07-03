# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_integ_msinx ${SRC})

  target_link_libraries(ex_integ_msinx
    ${LIBS}
    tubex-lib)


# Tests

  if(BUILD_TESTS)
  #  add_test(NAME ExampleIntegMsinx COMMAND ex_integ_msinx 0)
  endif(BUILD_TESTS)