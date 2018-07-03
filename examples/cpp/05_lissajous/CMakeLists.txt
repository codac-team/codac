# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_lissajous ${SRC})

  target_link_libraries(ex_lissajous
    ${LIBS}
    tubex-lib)


# Tests

  if(BUILD_TESTS)
  #  add_test(NAME ExampleLissajous COMMAND ex_lissajous 0)
  endif(BUILD_TESTS)