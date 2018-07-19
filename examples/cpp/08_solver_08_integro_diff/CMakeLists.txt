# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(solver_08_integro_diff ${SRC})

  target_link_libraries(solver_08_integro_diff
    ${LIBS}
    tubex-lib)


# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME solver_08_integro_diff COMMAND solver_08_integro_diff 0)
  #endif(BUILD_TESTS)