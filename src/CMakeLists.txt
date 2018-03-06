# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Headers stored into an include directory

  set(HEADERS_DIR ${CMAKE_CURRENT_BINARY_DIR}/../include)
  file(MAKE_DIRECTORY ${HEADERS_DIR})
  include_directories(${HEADERS_DIR})


# Sources

#  add_subdirectory(tube)
#  add_subdirectory(graphics)
#  add_subdirectory(tools)

  set(SRC
    tube/tubex_Tube.cpp
    tube/tubex_Contractor.cpp
    tube/tubex_Integration.cpp
    tube/tubex_Arithmetic.cpp
    tube/tubex_Serializer.cpp
    tube/exceptions/tubex_Exception.cpp
    tube/exceptions/tubex_DomainException.cpp
    tube/exceptions/tubex_EmptyException.cpp

    graphics/vibes/vibes.cpp
    graphics/tubex_VibesFigure.cpp
    graphics/tubex_VibesFigure_Tube.cpp

    tools/tubex_Serialization.cpp
  )

  set(HEADERS
    tube/tubex_Tube.h
    tube/exceptions/tubex_Exception.h
    tube/exceptions/tubex_DomainException.h
    tube/exceptions/tubex_EmptyException.h

    graphics/tubex_VibesFigure.h
    graphics/tubex_VibesFigure_Tube.h
    graphics/vibes/vibes.h

    tools/tubex_Serialization.h
  )

  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tubex-lib ${SRC})
  target_link_libraries(tubex-lib ${LIBS})

  install(FILES ${HEADERS} DESTINATION "include/tubex-lib")
  install(TARGETS tubex-lib DESTINATION lib)