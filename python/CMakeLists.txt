# ==================================================================
#  codac / Python binding - cmake configuration file
# ==================================================================

  add_subdirectory(pybind11)

  set(PYTHON_PACKAGE_NAME "codac")
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
      codac_py_core.cpp

      src/core/arithmetic/codac_py_arithmetic.cpp

      src/core/cn/codac_py_ContractorNetwork.cpp
      src/core/cn/codac_py_Variable.cpp

      src/core/contractors/static/codac_py_Ctc.cpp
      src/core/contractors/static/codac_py_CtcBox.cpp
      src/core/contractors/static/codac_py_CtcCartProd.cpp
      src/core/contractors/static/codac_py_CtcDist.cpp
      src/core/contractors/static/codac_py_CtcFunction.cpp
      src/core/contractors/static/codac_py_CtcConstell.cpp
      src/core/contractors/static/codac_py_CtcCN.cpp
      src/core/contractors/static/codac_py_CtcNewton.cpp
      src/core/contractors/static/codac_py_CtcPolar.cpp

      src/core/contractors/dyn/codac_py_DynCtc.cpp
      src/core/contractors/dyn/codac_py_CtcDelay.cpp
      src/core/contractors/dyn/codac_py_CtcDeriv.cpp
      src/core/contractors/dyn/codac_py_CtcEval.cpp
      src/core/contractors/dyn/codac_py_CtcLohner.cpp
      src/core/contractors/dyn/codac_py_CtcPicard.cpp
      src/core/contractors/dyn/codac_py_CtcStatic.cpp

      src/core/domains/interval/codac_py_bisectors.cpp
      src/core/domains/interval/codac_py_BoolInterval.cpp
      src/core/domains/interval/codac_py_Interval.cpp
      src/core/domains/interval/codac_py_IntervalVector.cpp
      src/core/domains/interval/codac_py_IntervalMatrix.cpp
      src/core/domains/tube/codac_py_Tube.cpp
      src/core/domains/tube/codac_py_TubeVector.cpp
      src/core/domains/slice/codac_py_Slice.cpp

      src/core/functions/codac_py_Function.cpp
      src/core/functions/codac_py_TFnc.cpp
      src/core/functions/codac_py_TFunction.cpp

      src/core/graphics/codac_py_graphics.cpp
      src/core/graphics/codac_py_ColorMap.cpp
      src/core/graphics/codac_py_Figure.cpp
      src/core/graphics/codac_py_VIBesFig.cpp
      src/core/graphics/codac_py_VIBesFigTube.cpp
      src/core/graphics/codac_py_VIBesFigTubeVector.cpp
      src/core/graphics/codac_py_VIBesFigMap.cpp
      src/core/graphics/codac_py_VIBesFigPaving.cpp

      src/core/geometry/codac_py_geometry.cpp
      
      src/core/paving/codac_py_Paving.cpp
      src/core/paving/codac_py_Set.cpp

      src/core/sivia/codac_py_sivia.cpp

      src/core/separators/codac_py_Sep.cpp

      src/core/variables/trajectory/codac_py_Trajectory.cpp
      src/core/variables/trajectory/codac_py_RandTrajectory.cpp
      src/core/variables/trajectory/codac_py_TrajectoryVector.cpp

      src/robotics/codac_py_DataLoader.cpp
      src/robotics/codac_py_TPlane.cpp
    )

    target_include_directories(core
      PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../include 
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
      ${CMAKE_CURRENT_BINARY_DIR}/docstring
    )

    target_link_libraries(core
      PRIVATE codac codac-rob ${LIBS}
    )

    # Copy the generated library in the package folder
    add_custom_command(TARGET core POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:core>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
    )

      # ex: from codac import core
      #     core.Tube
      # ex: from codac.core import Tube
      #     Tube

  # Generating a library unsupported.so containing the python binding:

    pybind11_add_module(unsupported SHARED
      codac_py_unsupported.cpp
      src/unsupported/contractors/codac_py_Ctc_unsupported.cpp
      src/unsupported/geometry/codac_geometry.cpp
      src/unsupported/image/codac_images.cpp
      src/unsupported/paving/codac_paving.cpp
      src/unsupported/separators/codac_py_Sep_unsupported.cpp
      src/unsupported/thickset/codac_thickset.cpp
      src/unsupported/thickset/codac_ThickInterval.cpp
    )

    target_include_directories(unsupported
      PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../include 
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
      ${CMAKE_CURRENT_BINARY_DIR}/docstring
    )

    target_link_libraries(unsupported
      PRIVATE core codac codac-unsupported core ${LIBS} core
    )

    # Copy the generated library in the package folder
    add_custom_command(TARGET unsupported POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:unsupported>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
    )


  #pybind11_add_module(graphics SHARED
  #  codac_py_VIBesFig.cpp
  #)
  #target_link_libraries(graphics PRIVATE codac codac-rob ${LIBS})
  #add_custom_command(TARGET graphics POST_BUILD
  #  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:graphics>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
  #)

  add_custom_target(pip_package)
  add_custom_command(TARGET pip_package PRE_BUILD
    COMMAND ${PYTHON_EXECUTABLE} ARGS setup.py bdist_wheel -d ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${PYTHON_PACKAGE_DIR}
  )


################################################################################
# Tests related to the binding
################################################################################

  if(BUILD_TESTS)
    include(CTest)
    file(GLOB TESTS_FILES
      "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/tests/test_*.py"
      "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/tests/tests_from_pyibex/test_*.py"
      )
    foreach(TEST_FILE ${TESTS_FILES})
      get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
      set(TEST_NAME "python_${TEST_NAME}")
      add_test(NAME ${TEST_NAME} COMMAND ${PYTHON_EXECUTABLE} ${TEST_FILE})
      set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHON_PACKAGE_DIR}")
    endforeach()

  endif()