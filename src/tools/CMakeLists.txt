# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    tubex_Serialization.cpp
  )

  set(HEADERS
    tubex_Serialization.h
  )

  file(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

  add_library(tubex-tools ${SRC})
  target_link_libraries(tubex-tools ${LIBS})

  install(FILES ${HEADERS} DESTINATION "include/tubex-lib")
  install(TARGETS tubex-tools DESTINATION lib)