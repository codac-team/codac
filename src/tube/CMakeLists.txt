# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    Tube_base.cpp
    Tube_ctc.cpp
    Tube_integration.cpp
    Tube_arithmetic.cpp
    Tube_serialization.cpp
    exceptions/TubeException.cpp
    exceptions/DomainTubeException.cpp
    exceptions/EmptyTubeException.cpp
  )

  set(HEADERS
    Tube.h
    exceptions/TubeException.h
    exceptions/DomainTubeException.h
    exceptions/EmptyTubeException.h
  )

  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tubex-core ${SRC})
  target_link_libraries(tubex-core ${LIBS} tubex-tools)

  install(FILES ${HEADERS} DESTINATION "include/tubex-lib")
  install(TARGETS tubex-core DESTINATION lib)