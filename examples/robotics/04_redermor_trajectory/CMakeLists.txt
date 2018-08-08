# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_redermor_traj ${SRC})

  target_link_libraries(ex_redermor_traj
    ${LIBS}
    tubex-core
    tubex-robotics)


# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME ex_redermor_traj COMMAND ex_redermor_traj 0)
  #endif(BUILD_TESTS)