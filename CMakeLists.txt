# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Settings

  cmake_minimum_required(VERSION 3.0)

  set(CMAKE_CXX_STANDARD  11)
  set(PACKAGE_NAME        tubex-lib)
  set(PACKAGE_VERSION     "2.0")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG") # comment for debug

  project(${PACKAGE_NAME})

  get_filename_component(TUBEX_ROOT          "./"         ABSOLUTE) # used for find_packages modules
  get_filename_component(TUBELIB_CMAKE_DIR   "./cmake"    ABSOLUTE)
  get_filename_component(TUBELIB_EXAMPLE_DIR "./examples" ABSOLUTE)
  get_filename_component(TUBELIB_BUILD_DIR   "./make"     ABSOLUTE)
  get_filename_component(TUBELIB_DATA_DIR    "./data"     ABSOLUTE)


# Dependencies

  list(APPEND CMAKE_MODULE_PATH "${TUBELIB_CMAKE_DIR}/modules") # location of find_package modules
  find_package(IbexLib REQUIRED) # will set variables IBEX_INCLUDE_DIR, IBEX_LIBRARY
  include_directories("${IBEX_INCLUDE_DIR}")


# Tubex sources

  add_subdirectory(src) # sources of core, robotics, solver
  include_directories(${TUBELIB_BUILD_DIR}/include/)


# Tests (unit tests + examples)

  option(BUILD_TESTS "Build test" OFF)
  if(BUILD_TESTS)
    enable_testing(true)

    # Unit tests
    add_subdirectory(tests)
    include(CTest)

    # Basics
    add_test(NAME basics_01
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_01_arithmetic/make/basics_01_arithmetic 0)
    add_test(NAME basics_02
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_02_simple_ctc/make/basics_02_simple_ctc 0)
    add_test(NAME basics_03
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_03_ivp_msinx/make/basics_03_ivp_msinx 0)
    add_test(NAME basics_04
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_04_lissajous/make/basics_04_lissajous 0)
    add_test(NAME basics_05
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_05_simple_eval/make/basics_05_simple_eval 0)
    add_test(NAME basics_06
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_06_graphics/make/basics_06_graphics 0)
    
    # Robotics
    add_test(NAME robotics_01
             COMMAND ${TUBELIB_EXAMPLE_DIR}/robotics/ex_01_causal_chain/make/rob_01_causal_chain 0)
    add_test(NAME robotics_02
             COMMAND ${TUBELIB_EXAMPLE_DIR}/robotics/ex_02_lowcost_beacons/make/rob_02_lowcost_beacons 0)
    add_test(NAME robotics_03
             COMMAND ${TUBELIB_EXAMPLE_DIR}/robotics/ex_03_drifting_clock/make/rob_03_drifting_clock 0)
    add_test(NAME robotics_04
             COMMAND sh -c "rm -f ${TUBELIB_DATA_DIR}/redermor/gesmi.txt.tubex ; ${TUBELIB_EXAMPLE_DIR}/robotics/ex_04_redermor_traj/make/rob_04_redermor_traj ${TUBELIB_DATA_DIR}/redermor/gesmi.txt 0")
    add_test(NAME robotics_04_with_serializ
             COMMAND ${TUBELIB_EXAMPLE_DIR}/robotics/ex_04_redermor_traj/make/rob_04_redermor_traj ${TUBELIB_DATA_DIR}/redermor/gesmi.txt 0)
    add_test(NAME robotics_05
             COMMAND ${TUBELIB_EXAMPLE_DIR}/robotics/ex_05_loops_detec/make/rob_05_loops_detec ${TUBELIB_DATA_DIR}/redermor/gesmi.txt 0)
    add_test(NAME robotics_06
             COMMAND ${TUBELIB_EXAMPLE_DIR}/robotics/ex_06_loops_proofs/make/rob_06_loops_proofs 0)
    add_test(NAME robotics_07
             COMMAND ${TUBELIB_EXAMPLE_DIR}/robotics/ex_07_dynloc/make/rob_07_dynloc 0)

    # Solver
    add_test(NAME solver_01
             COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_01_picard/make/solver_01_picard 0)
    add_test(NAME solver_02
             COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_02_xmsin_fwd/make/solver_02_xmsin_fwd 0)
    add_test(NAME solver_03
             COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_03_xmsin_bwd/make/solver_03_xmsin_bwd 0)
    add_test(NAME solver_04
             COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_04_bvp/make/solver_04_bvp 0)
    # add_test(NAME solver_05
    #          COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_05_delay/make/solver_05_delay 0)
    # add_test(NAME solver_06
    #          COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_06_bvp_delay/make/solver_06_bvp_delay 0)
    add_test(NAME solver_07
             COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_07_integro_diff/make/solver_07_integro_diff 0)
    # add_test(NAME solver_08
    #          COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_08_bvp_delay_2d/make/solver_08_bvp_delay_2d 0)
    add_test(NAME solver_09
             COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_09_csdp/make/solver_09_csdp 0)
    add_test(NAME solver_10
             COMMAND ${TUBELIB_EXAMPLE_DIR}/solver/ex_10_large_initvalue/make/solver_10_large_initvalue 0)

  endif()


# Uninstallation

  # todo