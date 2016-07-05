# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    Tube_base.cpp
    Tube_ctc.cpp
    Tube_integration.cpp
    Tube_arithmetic.cpp
  )

  set(HEADERS
    Tube.h
  )

  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tube-lib-core ${SRC})
  target_link_libraries(tube-lib-core ${LIBS})

  install(FILES ${HEADERS} DESTINATION "include/tube-lib")
  install(TARGETS tube-lib-core DESTINATION lib)


# Tests

  if(BUILD_TESTS)
  	add_subdirectory(test)
  endif(BUILD_TESTS)