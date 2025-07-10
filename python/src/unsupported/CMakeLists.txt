# ==================================================================
#  codac / Python binding - cmake configuration file
# ==================================================================

# Generating a library unsupported.so containing the python binding:
  list(APPEND PYBIND_UNSUPPORTED_SRC
      
    peibos/codac2_py_Parallelepiped.cpp
    peibos/codac2_py_Zonotope.cpp
    peibos/codac2_py_peibos.cpp
    codac2_py_unsupported.cpp
  )

  if (WITH_CAPD)
    list(APPEND PYBIND_UNSUPPORTED_SRC
      peibos/codac2_py_peibos_capd.cpp
    )
    # add_definitions(-DPEIBOS_CAPD)
  endif()

  pybind11_add_module(_unsupported SHARED
    ${PYBIND_UNSUPPORTED_SRC}
  )

  # if (WITH_CAPD)
  #   target_compile_definitions(_unsupported PRIVATE PEIBOS_CAPD)
  # endif()

  target_include_directories(_unsupported
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../docstring
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../../include 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
  )

  target_link_libraries(_unsupported
    PRIVATE ${PROJECT_NAME}-unsupported ${LIBS} Ibex::ibex
  )

  # Copy the generated library in the package folder
  add_custom_command(TARGET _unsupported POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_unsupported>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
  )

  # ex: from codac import unsupported
  #     unsupported.Tube
  # ex: from codac.unsupported import Tube
  #     Tube