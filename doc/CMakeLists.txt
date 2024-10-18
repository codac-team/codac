# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mathjax_stmaryrd.js DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mathjax_stmaryrd.js
        DESTINATION share/doc/${CMAKE_PROJECT_NAME}
        OPTIONAL)

# Technical API documentation (Doxygen)

  if(WITH_PYTHON)
    
    find_package(Doxygen 1.0.0)

    if(NOT DOXYGEN_FOUND)

      message(STATUS "Doxygen not found, not able to generate/install the API documentation.")

    else()

      if(NOT DOXYGEN_DIR)
        message(STATUS "Doxygen ${DOXYGEN_VERSION} found.")
      else()
        message(STATUS "Doxygen ${DOXYGEN_VERSION} found in ${DOXYGEN_DIR}.")
      endif()

      # Includes CMake commands in config file:
      configure_file(api/Doxyfile.in api/Doxyfile)

      set(DOXYGEN_INPUT ${CMAKE_CURRENT_BINARY_DIR}/api/Doxyfile)
      set(DOXYGEN_OUTPUT ${APIDOC_DIR}/html/index.html)

      if(WIN32)
        set(NULL_DEST NUL 2>&1)
      else()
        set(NULL_DEST "/dev/null")
      endif()

      #add_custom_target(api
      #                  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation... "
      #                  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT} > ${NULL_DEST}
      #                  COMMAND ${CMAKE_COMMAND} -E echo "done."
      #                  )

      message(STATUS "Building API Documentation (mandatory for Python binding)")
      execute_process(COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT} OUTPUT_QUIET)

      install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/api/html 
              DESTINATION share/doc/${CMAKE_PROJECT_NAME}/api
              OPTIONAL)

    endif()

  endif()