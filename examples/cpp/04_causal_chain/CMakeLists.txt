# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  set(HEADERS

  )

  add_executable(ex_causal_chain ${SRC})

  target_link_libraries(ex_causal_chain
    ${LIBS}
    tubex-lib)


# Tests

  if(BUILD_TESTS)
    add_test(NAME ExampleCausalChain COMMAND ex_causal_chain 0)
  endif(BUILD_TESTS)