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

    # Regenerate docstrings without changing unchanged files' timestamps.
    set(_docstring_dir "${CMAKE_CURRENT_BINARY_DIR}/docstring")
    set(_docstring_tmp "${CMAKE_CURRENT_BINARY_DIR}/docstring_tmp")
    file(REMOVE_RECURSE "${_docstring_tmp}")
    file(MAKE_DIRECTORY "${_docstring_dir}" "${_docstring_tmp}")
    message(STATUS "Generating docstrings from doxygen files...")
    execute_process(COMMAND ${PYTHON_EXECUTABLE}
      ${PROJECT_SOURCE_DIR}/scripts/doxygen/doxygen2docstring.py
      ${CMAKE_CURRENT_BINARY_DIR}/../doc/api/xml/
      "${_docstring_tmp}"
      RESULT_VARIABLE _docstring_status)
    if(NOT _docstring_status EQUAL 0)
      message(FATAL_ERROR "Failed to generate Python docstrings (exit code ${_docstring_status})")
    endif()

    file(GLOB _new_docstrings "${_docstring_tmp}/*")
    foreach(_file IN LISTS _new_docstrings)
      get_filename_component(_filename "${_file}" NAME)
      execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${_file}" "${_docstring_dir}/${_filename}"
        RESULT_VARIABLE _copy_status)
      if(NOT _copy_status EQUAL 0)
        message(FATAL_ERROR "Failed to update Python docstring: ${_filename}")
      endif()
    endforeach()

    # Remove headers that the generator no longer produces.
    file(GLOB _old_docstrings "${_docstring_dir}/*")
    foreach(_file IN LISTS _old_docstrings)
      get_filename_component(_filename "${_file}" NAME)
      if(NOT EXISTS "${_docstring_tmp}/${_filename}")
        file(REMOVE "${_file}")
      endif()
    endforeach()
    file(REMOVE_RECURSE "${_docstring_tmp}")

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

    # Copy Python tests at build time instead of reconfiguring CMake
    # whenever an individual test file changes.
    set(_python_test_outputs)
    foreach(TEST_FILE IN LISTS TESTS_FILES)
      string(REPLACE "codac2_tests_" "test_" TEST_FILE_UNITTEST "${TEST_FILE}") # unittest convention
      get_filename_component(TEST_FILE_UNITTEST "${TEST_FILE_UNITTEST}" NAME_WE)
      set(_test_output "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/tests/${TEST_FILE_UNITTEST}.py")
      add_custom_command(
        OUTPUT "${_test_output}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy "${TEST_FILE}" "${_test_output}"
        DEPENDS "${TEST_FILE}"
        VERBATIM
      )
      list(APPEND _python_test_outputs "${_test_output}")
    endforeach()
    add_custom_target(codac_python_test_files ALL DEPENDS ${_python_test_outputs})
    add_dependencies(pip_package codac_python_test_files)

  endif()
