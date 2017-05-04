# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    serialization.cpp
  )

  set(HEADERS
    serialization.h
  )

  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tubex-tools ${SRC})
  target_link_libraries(tubex-tools ${LIBS})

  install(FILES ${HEADERS} DESTINATION "include/tubex-lib")
  install(TARGETS tubex-tools DESTINATION lib)