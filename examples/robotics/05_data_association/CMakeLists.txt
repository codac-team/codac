# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  set(LIBS ${LIBS} -libex-geometry)
  link_directories(/usr/local/lib/ibex-geometry/)
  include_directories(/usr/local/include/ibex-geometry/)

  add_executable(ex_data_association ${SRC})

  target_link_libraries(ex_data_association
    ${LIBS}
    tubex-core
    tubex-robotics)

# Tests

  #if(BUILD_TESTS)
  #  add_test(NAME ex_data_association COMMAND ex_data_association 0)
  #endif(BUILD_TESTS)