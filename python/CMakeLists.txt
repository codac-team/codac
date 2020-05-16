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

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../include)
################################################################################
# Library for Python binding
################################################################################

# Generating a library tube.so containing the python binding:
pybind11_add_module(tube SHARED
  tubex_py.cpp
  tubex_py_contractors.cpp
  tubex_py_Slice.cpp
  tubex_py_TFunction.cpp
  tubex_py_Trajectory.cpp
  tubex_py_Tube.cpp
  tubex_py_TubeVector.cpp
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

pybind11_add_module(graphics SHARED
  tubex_py_VIBesFig.cpp
)
target_link_libraries(graphics PRIVATE tubex tubex-rob ${LIBS})
add_custom_command(TARGET graphics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:graphics>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
)



################################################################################
# Tests related to the binding
################################################################################

# SET (CTEST_ENVIRONMENT PYTHONPATH=${PYTHON_PACKAGE_DIR})
# add_test(NAME python_binding
#   COMMAND ${PYTHON_EXECUTABLE} -m unittest discover ${PYTHON_PACKAGE_NAME}
#   WORKING_DIRECTORY ${PYTHON_PACKAGE_DIR}
# )

file(GLOB TESTS_FILES "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/tests/test_*.py")
foreach(TEST_FILE ${TESTS_FILES})
	# FILE(COPY ${TEST_FILE} DESTINATION .)
	GET_FILENAME_COMPONENT(baseName ${TEST_FILE} NAME_WE)
	ADD_TEST(${baseName} ${PYTHON_EXECUTABLE} ${TEST_FILE})
	SET_TESTS_PROPERTIES(${baseName}
	PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHON_PACKAGE_DIR}")

# PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHON_INSTDIR}:${CMAKE_CURRENT_BINARY_DIR}/../src/python_package")
    # SET_TESTS_PROPERTIES(${baseName}
        # PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${LIBRARY_OUTPUT_PATH}")
 endforeach()