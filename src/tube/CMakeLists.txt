# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    tubex_Tube.cpp
    tubex_Contractor.cpp
    tubex_Integration.cpp
    tubex_Arithmetic.cpp
    tubex_Serializer.cpp
    exceptions/tubex_Exception.cpp
    exceptions/tubex_DomainException.cpp
    exceptions/tubex_EmptyException.cpp
  )

  set(HEADERS
    tubex_Tube.h
    exceptions/tubex_Exception.h
    exceptions/tubex_DomainException.h
    exceptions/tubex_EmptyException.h
  )

  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tubex-core ${SRC})
  target_link_libraries(tubex-core ${LIBS} tubex-tools)

  install(FILES ${HEADERS} DESTINATION "include/tubex-lib")
  install(TARGETS tubex-core DESTINATION lib)