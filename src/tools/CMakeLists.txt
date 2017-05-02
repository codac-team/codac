# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    serialization.cpp
  )

  set(HEADERS
    serialization.h
  )

  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tube-tools ${SRC})
  target_link_libraries(tube-tools ${LIBS})

  install(FILES ${HEADERS} DESTINATION "include/tube-lib")
  install(TARGETS tube-tools DESTINATION lib)