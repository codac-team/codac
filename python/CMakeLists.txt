# ==================================================================
#  codac / Python binding - cmake configuration file
# ==================================================================

  option(ENABLE_FIND_PACKAGE_PYBIND11 "ENABLE_FIND_PACKAGE_PYBIND11" ON)
  if(ENABLE_FIND_PACKAGE_PYBIND11)
    find_package(pybind11 3.0.1 QUIET)
  endif()
  if(pybind11_FOUND)
    message(STATUS "Found pybind11 version ${pybind11_VERSION} in ${pybind11_DIR}") # Use pybind11_DIR or CMAKE_PREFIX_PATH to specify a custom pybind11 installation path...
  else()
    message(STATUS "Attempt to install pybind11 automatically...")
    FetchContent_Declare(pybind11 URL https://github.com/pybind/pybind11/archive/refs/tags/v3.0.1.zip)
    #FetchContent_Declare(pybind11 URL ${CMAKE_CURRENT_SOURCE_DIR}/../3rd/pybind11-ed5057ded698e305210269dafa57574ecf964483.zip) # If needed to be self-contained...
    FetchContent_MakeAvailable(pybind11)
  endif()
  # Adds pybind11::headers, pybind11::module, pybind11::embed

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

  option(NO_DOCSTRING_UPDATE "No docstrings update" OFF)

  if(NO_DOCSTRING_UPDATE)

    # ^ This option allows a faster compilation of the library in case of minor changes
    #   by disabling the generation of docstring files for Python, and so the re-compilation
    #   of all the related python binding files.  
    message(STATUS "Generation of docstrings is disabled.")

  else()

    set(NO_DOCSTRING_UPDATE OFF CACHE BOOL "Disables the update of docstrings")

    file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/docstring)
    message(STATUS "Generating docstrings from doxygen files...")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docstring)
    execute_process(COMMAND ${PYTHON_EXECUTABLE}
      ${PROJECT_SOURCE_DIR}/scripts/doxygen/doxygen2docstring.py
      ${CMAKE_CURRENT_BINARY_DIR}/../doc/api/xml/
      ${CMAKE_CURRENT_BINARY_DIR}/docstring)

  endif()


################################################################################
# Library for Python binding
################################################################################

  add_subdirectory(src/core)
  add_subdirectory(src/graphics)
  add_subdirectory(src/unsupported)

  
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
      "${CMAKE_CURRENT_SOURCE_DIR}/../tests/codac2_tests_*.py"
      )

    foreach(TEST_FILE ${TESTS_FILES})
      string(REPLACE "codac2_tests_" "test_" TEST_FILE_UNITTEST ${TEST_FILE}) # unittest convention
      get_filename_component(TEST_FILE_UNITTEST ${TEST_FILE_UNITTEST} NAME_WE)
      configure_file(${TEST_FILE} ${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/tests/${TEST_FILE_UNITTEST}.py COPYONLY)
      #get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
      #set(TEST_NAME "python_${TEST_NAME}")
      #add_test(NAME ${TEST_NAME} COMMAND ${PYTHON_EXECUTABLE} ${TEST_FILE})
      #set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHON_PACKAGE_DIR}")
    endforeach()

  endif()