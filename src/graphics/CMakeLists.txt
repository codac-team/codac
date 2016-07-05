# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    vibes/vibes.cpp
    VibesFigure.cpp
    VibesFigure_Tube.cpp
  )

  set(HEADERS
    VibesFigure.h
    VibesFigure_Tube.h
    vibes/vibes.h
  )

  include_directories(vibes)
  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tube-graphics ${SRC})
  target_link_libraries(tube-graphics ${LIBS})

  install(FILES ${HEADERS} DESTINATION "include/tube-lib")
  install(TARGETS tube-graphics DESTINATION lib)