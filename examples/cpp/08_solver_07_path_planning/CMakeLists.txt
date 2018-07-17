# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(solver_07_path_planning ${SRC})

  target_link_libraries(solver_07_path_planning
    ${LIBS}
    tubex-lib)


# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME solver_07_path_planning COMMAND solver_07_path_planning 0)
  #endif(BUILD_TESTS)