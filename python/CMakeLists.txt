# ==================================================================
#  tubex-lib / Python binding - cmake configuration file
# ==================================================================

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
  # Library for Python binding
  ################################################################################

  # Generating a library tube.so containing the python binding:
  pybind11_add_module(tube SHARED
    tubex_py.cpp
    tubex_py_contractors.cpp
    tubex_py_ContractorNetwork.cpp
    tubex_py_Slice.cpp
    tubex_py_TFunction.cpp
    tubex_py_Trajectory.cpp
    tubex_py_Tube.cpp
    tubex_py_TubeVector.cpp
    tubex_py_VIBesFig.cpp
    tubex_py_DataLoader.cpp
    tubex_py_arithmetic.cpp
  )

  target_include_directories(tube
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../include)

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