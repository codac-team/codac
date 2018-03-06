# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    vibes/vibes.cpp
    tubex_VibesFigure.cpp
    tubex_VibesFigure_Tube.cpp
  )

  set(HEADERS
    tubex_VibesFigure.h
    tubex_VibesFigure_Tube.h
    vibes/vibes.h
  )

  include_directories(vibes)
  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tubex-graphics ${SRC})
  target_link_libraries(tubex-graphics ${LIBS})

  install(FILES ${HEADERS} DESTINATION "include/tubex-lib")
  install(TARGETS tubex-graphics DESTINATION lib)