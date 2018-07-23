# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(solver_10_csdp ${SRC})

  target_link_libraries(solver_10_csdp
    ${LIBS}
    tubex-core
    tubex-solver)


# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME solver_10_csdp COMMAND solver_10_csdp 0)
  #endif(BUILD_TESTS)