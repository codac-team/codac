# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_memory ${SRC})

  target_link_libraries(ex_memory
    ${LIBS}
    tubex-lib)


# Tests

  if(BUILD_TESTS)
  #  add_test(NAME ExampleMemory COMMAND ex_memory 0)
  endif(BUILD_TESTS)