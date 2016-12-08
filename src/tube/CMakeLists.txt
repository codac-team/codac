# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    exceptions/TubeException.cpp
    exceptions/DomainTubeException.cpp
    exceptions/EmptyTubeException.cpp
    Tube_base.cpp
    Tube_ctc.cpp
    Tube_integration.cpp
    Tube_arithmetic.cpp
    Tube_serialization.cpp
  )

  set(HEADERS
    Tube.h
    exceptions/TubeException.h
    exceptions/DomainTubeException.h
    exceptions/EmptyTubeException.h
  )

  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tube-core ${SRC})
  target_link_libraries(tube-core ${LIBS})

  install(FILES ${HEADERS} DESTINATION "include/tube-lib")
  install(TARGETS tube-core DESTINATION lib)