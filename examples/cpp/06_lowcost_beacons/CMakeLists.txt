# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_lowcost_beacons ${SRC})

  target_link_libraries(ex_lowcost_beacons
    ${LIBS}
    tubex-lib)


# Tests

  if(BUILD_TESTS)
    add_test(NAME ExampleLowcostBeacons COMMAND ex_lowcost_beacons 0)
  endif(BUILD_TESTS)