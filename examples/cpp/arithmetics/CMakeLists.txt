# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_arithmetics ${SRC})

  target_link_libraries(ex_arithmetics
    ${LIBS}
    tube-core
    tube-graphics)


# Tests

  if(BUILD_TESTS)
    add_test(ex_arithmetics ex_arithmetics 0)
  endif(BUILD_TESTS)