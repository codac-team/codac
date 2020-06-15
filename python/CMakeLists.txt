# ==================================================================
#  tubex-lib / Python binding - cmake configuration file
# ==================================================================

  add_subdirectory(pybind11)

  set(PYTHON_PACKAGE_NAME "tubex_lib")
  set(PYTHON_PACKAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python_package")

  file(MAKE_DIRECTORY ${PYTHON_PACKAGE_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_PACKAGE_NAME}/" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}")

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
    if(NOT WIN32)
      string(ASCII 27 Esc)
      set(ColourReset "${Esc}[m")
      set(Red         "${Esc}[31m")
      set(Blue        "${Esc}[34m")
    endif()

    message(STATUS "${Red}/!\\ Run \"make api\" for generating Doxygen doc, then \"cmake ..\" again, before \"make\"${ColourReset}")
  endif()

  message(STATUS "Generating docstrings from doxygen files...")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docstring)
  execute_process(COMMAND ${PYTHON_EXECUTABLE}
    ${PROJECT_SOURCE_DIR}/scripts/pybind/doxygen2docstring.py
    ${CMAKE_CURRENT_BINARY_DIR}/../doc/api/xml/
    ${CMAKE_CURRENT_BINARY_DIR}/docstring)
  # The file generated_docstring.cpp contains all docstring of the library


  ################################################################################
  # Library for Python binding
  ################################################################################

  # Generating a library tube.so containing the python binding:
  pybind11_add_module(tube SHARED
    tubex_py.cpp

    src/core/arithmetic/tubex_py_arithmetic.cpp
    src/core/cn/tubex_py_ContractorNetwork.cpp

    src/core/contractors/static/tubex_py_CtcDist.cpp
    src/core/contractors/static/tubex_py_CtcFunction.cpp
    src/core/contractors/static/tubex_py_CtcConstell.cpp

    src/core/contractors/dyn/tubex_py_DynCtc.cpp
    src/core/contractors/dyn/tubex_py_CtcDeriv.cpp
    src/core/contractors/dyn/tubex_py_CtcEval.cpp
    src/core/contractors/dyn/tubex_py_CtcPicard.cpp

    src/core/dynamics/tube/tubex_py_Tube.cpp
    src/core/dynamics/tube/tubex_py_TubeVector.cpp
    src/core/dynamics/trajectory/tubex_py_Trajectory.cpp
    src/core/dynamics/trajectory/tubex_py_RandTrajectory.cpp
    src/core/dynamics/trajectory/tubex_py_TrajectoryVector.cpp
    src/core/dynamics/slice/tubex_py_Slice.cpp

    src/core/functions/tubex_py_TFnc.cpp
    src/core/functions/tubex_py_TFunction.cpp

    src/core/graphics/tubex_py_graphics.cpp
    src/core/graphics/tubex_py_ColorMap.cpp
    src/core/graphics/tubex_py_Figure.cpp
    src/core/graphics/tubex_py_VIBesFig.cpp
    src/core/graphics/tubex_py_VIBesFigTube.cpp
    src/core/graphics/tubex_py_VIBesFigTubeVector.cpp
    src/core/graphics/tubex_py_VIBesFigMap.cpp

    src/robotics/tubex_py_DataLoader.cpp
  )

  target_include_directories(tube
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../include 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/docstring)

  target_link_libraries(tube
    PRIVATE tubex tubex-rob ${LIBS})

    # ex: from tubex_lib import tube
    #     tube.Tube
    # ex: from tubex_lib.tube import Tube
    #     Tube

  # Copy the generated library in the package folder
  add_custom_command(TARGET tube POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:tube>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
  )

  #pybind11_add_module(graphics SHARED
  #  tubex_py_VIBesFig.cpp
  #)
  #target_link_libraries(graphics PRIVATE tubex tubex-rob ${LIBS})
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

    file(GLOB TESTS_FILES "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/tests/test_*.py")
    foreach(TEST_FILE ${TESTS_FILES})
      get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
      set(TEST_NAME "python_${TEST_NAME}")
      add_test(NAME ${TEST_NAME} COMMAND ${PYTHON_EXECUTABLE} ${TEST_FILE})
      set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHON_PACKAGE_DIR}")
    endforeach()

  endif()