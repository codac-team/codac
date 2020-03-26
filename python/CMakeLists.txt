# ==================================================================
#  tubex-lib / Python binding - cmake configuration file
# ==================================================================

set(PYTHON_PACKAGE_NAME "pyibex_tube")
set(PYTHON_PACKAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python_package")

file(MAKE_DIRECTORY ${PYTHON_PACKAGE_DIR})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_PACKAGE_NAME}/" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}")

# Installation process is handled by setup.py
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
  ${PYTHON_PACKAGE_DIR}/setup.py
)


################################################################################
# Library for Python binding
################################################################################

# Generating a library tube.so containing the python binding:
pybind11_add_module(tube SHARED
  pyibex_tube.cpp
  pyibex_export_Function.cpp
  pyibex_export_Slice.cpp
  pyibex_export_Trajectory.cpp
  pyibex_export_Tube.cpp
  pyibex_export_TubeVector.cpp

)
target_link_libraries(tube PRIVATE tubex ${LIBS})

  # ex: from pyibex_tube import tube
  #     tube.Tube
  # ex: from pyibex_tube.tube import Tube
  #     Tube

# Copy the generated library in the package folder
add_custom_command(TARGET tube POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:tube>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
)


################################################################################
# Tests related to the binding
################################################################################

add_test(NAME python_binding
  COMMAND ${PYTHON_EXECUTABLE} -m unittest discover ${PYTHON_PACKAGE_NAME}.tests
  WORKING_DIRECTORY ${PYTHON_PACKAGE_DIR}
)