# ==================================================================
#  codac / Python binding - cmake configuration file
# ==================================================================

# Generating a library graphics.so containing the python binding:

  pybind11_add_module(_graphics SHARED
    codac2_py_graphics.cpp

    figures/codac2_py_Figure2D.cpp
    figures/codac2_py_Figure3D.cpp

    styles/codac2_py_Color.cpp
    styles/codac2_py_ColorMap.cpp
    styles/codac2_py_PavingStyle.cpp
    styles/codac2_py_StyleGradientProperties.cpp
    styles/codac2_py_StyleProperties.cpp
  )

  target_include_directories(_graphics
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../docstring
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../../include 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/figures/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/styles/
  )

  target_link_libraries(_graphics
    PRIVATE ${PROJECT_NAME}-graphics ${LIBS} Ibex::ibex
  )

  # Copy the generated library in the package folder
  add_custom_command(TARGET _graphics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_graphics>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
  )

  # ex: from codac import graphics
  #     graphics.Tube
  # ex: from codac.graphics import Tube
  #     Tube