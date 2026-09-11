# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

# Testing examples:

if(BUILD_TESTS AND TEST_EXAMPLES)

  # Every example ships a standalone CMakeLists.txt, so that a user can build it
  # on its own against an installed codac. Those projects cannot be pulled in
  # with add_subdirectory(): each calls project() and find_package(CODAC), which
  # would look for a codac that is precisely the one being built. The examples
  # are therefore rebuilt here from the same sources, against the in-tree
  # library, the way tests/CMakeLists.txt builds the unit tests. Only the public
  # headers are visible to them -- the generated umbrella "codac" and the
  # installed headers next to it -- so an example that stops compiling here is
  # an example a user could no longer build either.
  #
  # The lists below mirror the add_executable() calls of those standalone
  # CMakeLists.txt: an example directory may hold sources that its own project
  # does not build (02_centered_form/main_parabolas.cpp, for instance), and
  # those are deliberately left out rather than silently compiled.

  set(CODAC_EXAMPLES_CPP
    00_graphics/graphic_examples
    00_graphics/graphic_animation
    01_batman/main
    02_centered_form/main
    03_sivia/main
    04_explored_area/main
    06_graphics_3D/main
    07_centered_2D/main
    08_centered_3D/main
    09_robot_simu/main
    11_peibos/main
    13_qinter/main
    14_lohner/main
    16_visibility/main
    ellipsoid_example/main
  )

  set(CODAC_EXAMPLES_PY
    00_graphics/graphic_examples
    00_graphics/graphic_animation
    00_graphics/graphic_colors
    02_centered_form/main
    02_centered_form/main_parabolas
    03_sivia/main
    04_explored_area/main
    06_graphics_3D/main
    09_robot_simu/main
    11_peibos/main
    13_qinter/main
    14_lohner/main
    16_visibility/main
    custom_sep/coloration
    custom_sep/custom_ctc
    custom_sep/custom_sep
    ellipsoid_example/main
  )

  set(CODAC_EXAMPLE_LIBRARIES ${PROJECT_NAME}-core ${PROJECT_NAME}-graphics)

  if(WITH_CAPD)
    # 10_lie_groups is in this list rather than in the one above because it
    # includes <codac-capd.h>, which its own CMakeLists.txt does not say: the
    # standalone project builds it only where a codac with CAPD is installed.
    list(APPEND CODAC_EXAMPLES_CPP
      05_capd_solver/main
      10_lie_groups/lie_01
      12_peibos_capd/main
    )
    list(APPEND CODAC_EXAMPLE_LIBRARIES ${PROJECT_NAME}-capd capd::capd)
  endif()

  # BUILD_SYMPY_EMBED_TESTS is decided in tests/CMakeLists.txt, which the
  # top-level CMakeLists.txt adds before this directory. It is turned off
  # wherever PYBIND11_FINDPYTHON=OFF, which is every configuration this
  # project's CI builds, so the sympy example follows the sympy unit test
  # rather than being built on its own terms.
  if(WITH_PYTHON AND BUILD_SYMPY_EMBED_TESTS)
    list(APPEND CODAC_EXAMPLES_CPP 15_sympy/main)
    list(APPEND CODAC_EXAMPLES_PY 15_sympy/main)
    list(APPEND CODAC_EXAMPLE_LIBRARIES ${PROJECT_NAME}-sympy)
  endif()

  # The examples write their figures (JSON for VIBes, .ipe, .png, .obj) into the
  # current directory, so they are run from the build tree rather than from the
  # sources they were copied from. Assets they read are found through __FILE__,
  # which stays an absolute path, so nothing has to be copied over.
  set(CODAC_EXAMPLES_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/output)
  file(MAKE_DIRECTORY ${CODAC_EXAMPLES_OUTPUT_DIR})

  foreach(SRC_EXAMPLE ${CODAC_EXAMPLES_CPP})

    string(REPLACE "/" "_" EXAMPLE_NAME ${SRC_EXAMPLE})
    set(EXAMPLE_NAME codac2_examples_${EXAMPLE_NAME})

    add_executable(
      ${EXAMPLE_NAME}
      ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_EXAMPLE}.cpp
    )

    target_include_directories(
      ${EXAMPLE_NAME}
      SYSTEM PUBLIC

        # First, so that a header is opened at src/... rather than through
        # build/include: the path a translation unit used is the path gcov records,
        # and the two spellings would split every header into two entries in the
        # coverage report. See codac_publish_include_dirs() in the top-level CMakeLists.txt.
        ${CODAC_SOURCE_INCLUDE_DIRS}

        ${CMAKE_BINARY_DIR}/src      # the generated "codac" umbrella header and module headers (codac-core.h, codac-graphics.h, etc.)
        ${CMAKE_BINARY_DIR}/src/core
        ${CMAKE_BINARY_DIR}/src/graphics
    )

    target_link_libraries(
      ${EXAMPLE_NAME}
      PUBLIC

        Ibex::ibex
        ${CODAC_EXAMPLE_LIBRARIES}
    )

    # Propager les flags d'architecture (FMA, etc.) vers les exemples (Option 2)
    if(CODAC_ARCH_CXX_FLAGS)
      target_compile_options(${EXAMPLE_NAME} PRIVATE ${CODAC_ARCH_CXX_FLAGS})
    endif()

    add_test(
      NAME ${EXAMPLE_NAME}_cpp
      COMMAND ${EXAMPLE_NAME}
      WORKING_DIRECTORY ${CODAC_EXAMPLES_OUTPUT_DIR}
    )

    if(CODAC_CPP_TEST_ENV)
      set_tests_properties(${EXAMPLE_NAME}_cpp PROPERTIES
        ENVIRONMENT "${CODAC_CPP_TEST_ENV}"
      )
    endif()

    # The examples are part of the ctest suite here, so `make check` -- which
    # rebuilds the suite before running it -- has to build them too. Without
    # this it would run ctest against executables that may not exist yet, the
    # way it does for the unit tests through the same call in
    # tests/CMakeLists.txt.
    if(TARGET check)
      add_dependencies(check ${EXAMPLE_NAME})
    endif()

  endforeach()

  if(WITH_PYTHON)

    foreach(SRC_EXAMPLE ${CODAC_EXAMPLES_PY})

      string(REPLACE "/" "_" EXAMPLE_NAME ${SRC_EXAMPLE})
      set(EXAMPLE_NAME codac2_examples_${EXAMPLE_NAME})

      # PYTHON_TEST_ENV_ARGS carries whatever the Python interpreter needs to
      # load a sanitized or a Windows build (LD_PRELOAD, PATH...); it is built
      # in tests/CMakeLists.txt and exported from there.
      add_test(
        NAME ${EXAMPLE_NAME}_py
        COMMAND
          ${CMAKE_COMMAND} -E env
          ${PYTHON_TEST_ENV_ARGS}
          ${PYTHON_EXECUTABLE}
          ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_EXAMPLE}.py
        WORKING_DIRECTORY ${CODAC_EXAMPLES_OUTPUT_DIR}
      )

    endforeach()

  endif()

endif()
