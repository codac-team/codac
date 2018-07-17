# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(solver_04_bvp ${SRC})

  target_link_libraries(solver_04_bvp
    ${LIBS}
    tubex-lib)


# Tests

  if(BUILD_TESTS)
    add_test(NAME solver_04_bvp COMMAND solver_04_bvp 0)
  endif(BUILD_TESTS)