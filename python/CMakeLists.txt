# ==================================================================
#  codac / Python binding - cmake configuration file
# ==================================================================

  add_subdirectory(pybind11)

  set(PYTHON_PACKAGE_NAME ${PROJECT_NAME})
  set(PYTHON_PACKAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python_package")

  file(MAKE_DIRECTORY ${PYTHON_PACKAGE_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_PACKAGE_NAME}/" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}")
  file(GENERATE OUTPUT "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/version.py" CONTENT "__version__=\"${PROJECT_VERSION}\"")
  
  if(NOT WIN32)
    set(FORCE_PYTHON_EXT "ext_modules=[Extension(\"${PYTHON_PACKAGE_NAME}.empty\", sources=[\"empty.c\"])],")
    # trick to generate a correct wheel.
    file(WRITE "${PYTHON_PACKAGE_DIR}/empty.c" "// empty source file")
  endif()

  # Installation process is handled by setup.py
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
    ${PYTHON_PACKAGE_DIR}/setup.py
  )


  ################################################################################
  # Generating docstring documentation from Doxygen
  ################################################################################

  if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../doc/api/xml)
    # The file generated_docstring.cpp contains all docstring of the library
    message(FATAL_ERROR "Unable to find Doxygen generated files, this is required for Python binding (docstring)")
  endif()

  file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/docstring)
  message(STATUS "Generating docstrings from doxygen files...")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docstring)
  execute_process(COMMAND ${PYTHON_EXECUTABLE}
    ${PROJECT_SOURCE_DIR}/scripts/pybind/doxygen2docstring.py
    ${CMAKE_CURRENT_BINARY_DIR}/../doc/api/xml/
    ${CMAKE_CURRENT_BINARY_DIR}/docstring)


  ################################################################################
  # Library for Python binding
  ################################################################################

  # Generating a library core.so containing the python binding:

    pybind11_add_module(core SHARED
      codac2_py_core.cpp
      codac2_py_core.h
      
      src/core/contractors/codac2_py_Ctc.cpp
      src/core/contractors/codac2_py_Ctc.h
      src/core/contractors/codac2_py_CtcInter.cpp
      src/core/contractors/codac2_py_CtcInverse.h
      src/core/contractors/codac2_py_CtcUnion.cpp
      src/core/contractors/codac2_py_CtcWrapper.cpp

      src/core/domains/interval/codac2_py_Interval.cpp
      src/core/domains/interval/codac2_py_Interval_operations.cpp
      src/core/domains/interval/codac2_py_IntervalMatrix.cpp
      src/core/domains/interval/codac2_py_IntervalMatrix_operations.cpp
      src/core/domains/interval/codac2_py_IntervalMatrixBase.h
      src/core/domains/interval/codac2_py_IntervalVector.cpp
      src/core/domains/interval/codac2_py_IntervalVector_operations.cpp
      src/core/domains/codac2_py_BoolInterval.cpp

      src/core/functions/analytic/codac2_py_analytic_operations.cpp
      src/core/functions/analytic/codac2_py_analytic_variables.cpp
      src/core/functions/analytic/codac2_py_AnalyticFunction.h
      src/core/functions/analytic/codac2_py_ExprWrapper.h

      src/core/graphics/codac2_py_Figure2D.cpp
      src/core/graphics/codac2_py_StyleProperties.cpp

      src/core/matrices/codac2_py_Matrix.cpp
      src/core/matrices/codac2_py_MatrixBase.h
      src/core/matrices/codac2_py_Vector.cpp
      src/core/matrices/codac2_py_VectorBase.h

      src/core/paver/codac2_py_Paver.cpp

      src/core/separators/codac2_py_Sep.cpp
      src/core/separators/codac2_py_Sep.h
      src/core/separators/codac2_py_SepInter.cpp
      src/core/separators/codac2_py_SepInverse.h
      src/core/separators/codac2_py_SepPolygon.cpp
      src/core/separators/codac2_py_SepUnion.cpp

      src/core/tools/codac2_py_Approx.cpp
    )

    target_include_directories(core
      PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../include 
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/contractors/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/domains/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/domains/interval/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/functions/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/functions/analytic/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/matrices/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/paver/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/separators/
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/tools/
      ${CMAKE_CURRENT_BINARY_DIR}/docstring
    )

   	target_link_libraries(core
   	  PRIVATE ${PROJECT_NAME}-core ${LIBS} Ibex::ibex
   	)

    # Copy the generated library in the package folder
    add_custom_command(TARGET core POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:core>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
    )

    # ex: from codac import core
    #     core.Tube
    # ex: from codac.core import Tube
    #     Tube

  # Generating a library graphics.so containing the python binding:

    pybind11_add_module(graphics SHARED
      codac2_py_graphics.cpp
    )

    target_include_directories(graphics
      PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../include 
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
      ${CMAKE_CURRENT_BINARY_DIR}/docstring
    )

    target_link_libraries(graphics
      PRIVATE ${PROJECT_NAME}-core ${LIBS} Ibex::ibex
    )

    # Copy the generated library in the package folder
    add_custom_command(TARGET graphics POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:graphics>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
    )

  # Generating a library unsupported.so containing the python binding:

    pybind11_add_module(unsupported SHARED
      codac2_py_unsupported.cpp
    )

    target_include_directories(unsupported
      PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../include 
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
      ${CMAKE_CURRENT_BINARY_DIR}/docstring
    )

    target_link_libraries(unsupported
      PRIVATE ${PROJECT_NAME}-core ${LIBS} Ibex::ibex
    )

    # Copy the generated library in the package folder
    add_custom_command(TARGET unsupported POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:unsupported>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
    )

  add_custom_target(pip_package)
  add_custom_command(TARGET pip_package PRE_BUILD
    COMMAND ${PYTHON_EXECUTABLE} ARGS setup.py bdist_wheel -d ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${PYTHON_PACKAGE_DIR}
  )


################################################################################
# Tests related to the binding
################################################################################

  if(BUILD_TESTS)
    
    file(GLOB_RECURSE TESTS_FILES
      "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/../../tests/core/codac2_tests_*.py"
      )
    foreach(TEST_FILE ${TESTS_FILES})
      get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
      set(TEST_NAME "python_${TEST_NAME}")
      add_test(NAME ${TEST_NAME} COMMAND ${PYTHON_EXECUTABLE} ${TEST_FILE})
      set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHON_PACKAGE_DIR}")
    endforeach()

  endif()