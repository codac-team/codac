# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Testing examples:

if(BUILD_TESTS AND TEST_EXAMPLES)

#  todo: compilation of the examples from CMake (remove ./make all alternative)

#  include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
#
#  list(APPEND CMAKE_ARGS "-DTUBEX_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../src/")
#  list(APPEND CMAKE_ARGS "-DTUBEX_LIBRARY=${CMAKE_CURRENT_SOURCE_DIR}/../src/core/libtubex.a")
#  list(APPEND CMAKE_ARGS "-DTUBEX_ROB_LIBRARY=${CMAKE_CURRENT_SOURCE_DIR}/../src/robotics/libtubex-rob.a")
#  list(APPEND CMAKE_ARGS "-DTUBEX_ODE_LIBRARY=${CMAKE_CURRENT_SOURCE_DIR}/../src/3rd/ode/libtubex-ode.a")
#  list(APPEND CMAKE_ARGS "-DTUBEX_PYIBEX_LIBRARY=${CMAKE_CURRENT_SOURCE_DIR}/../src/3rd/pyibex/libtubex-pyibex.a")
#  list(APPEND CMAKE_ARGS "-DTUBEX_INCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../include/")
#  list(APPEND CMAKE_ARGS "-DTUBEX_ROB_INCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../include/")
#  list(APPEND CMAKE_ARGS "-DTUBEX_ODE_INCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../include/")
#  list(APPEND CMAKE_ARGS "-DTUBEX_PYIBEX_INCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../include/")
#
#  list(APPEND TUBEX_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../src/core/libtubex.a")
#  list(APPEND TUBEX_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../src/robotics/libtubex-rob.a")
#  list(APPEND TUBEX_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../src/3rd/ode/libtubex-ode.a")
#  list(APPEND TUBEX_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../src/3rd/pyibex/libtubex-pyibex.a")
#
#  list(APPEND CMAKE_ARGS "-DTUBEX_INCLUDE_DIRS=${CMAKE_CURRENT_SOURCE_DIR}/../include")
#  list(APPEND CMAKE_ARGS "-DTUBEX_LIBRARIES=${TUBEX_LIBRARIES}")
#
#  ExternalProject_Add(basics_01
#                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/basics/ex_01_arithmetic
#                      CMAKE_ARGS ${CMAKE_ARGS}
#                      INSTALL_COMMAND cmake -E echo "Skipping install step.")


  # Tuto
  add_test(NAME cpp_01_getting_started
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tuto/01_getting_started/build/01_getting_started 0)
  if(WITH_PYTHON)
  add_test(NAME py_01_getting_started
           COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tuto/01_getting_started/01_getting_started.py 0)
  endif()

  add_test(NAME cpp_02_static_rangeonly
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tuto/02_static_rangeonly/build/02_static_rangeonly 0)
  if(WITH_PYTHON)
  add_test(NAME py_02_static_rangeonly
           COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tuto/02_static_rangeonly/02_static_rangeonly.py 0)
  endif()

  add_test(NAME cpp_03_static_rangebearing
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tuto/03_static_rangebearing/build/03_static_rangebearing 0)
  if(WITH_PYTHON)
  add_test(NAME py_03_static_rangebearing
           COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tuto/03_static_rangebearing/03_static_rangebearing.py 0)
  endif()

  add_test(NAME cpp_04_dyn_rangeonly
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tuto/04_dyn_rangeonly/build/04_dyn_rangeonly 0)
  if(WITH_PYTHON)
  add_test(NAME py_04_dyn_rangeonly
           COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tuto/04_dyn_rangeonly/04_dyn_rangeonly.py 0)
  endif()

  add_test(NAME cpp_05_dyn_rangebearing
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tuto/05_dyn_rangebearing/build/05_dyn_rangebearing 0)
  if(WITH_PYTHON)
  add_test(NAME py_05_dyn_rangebearing
           COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tuto/05_dyn_rangebearing/05_dyn_rangebearing.py 0)
  endif()
  
  # Basics
  #add_test(NAME basics_01
  #         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/ex_01_arithmetic/build/tubex_basics_01 0)
  add_test(NAME basics_02
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/ex_02_simple_ctc/build/tubex_basics_02 0)
  add_test(NAME basics_03
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/ex_03_ivp_msinx/build/tubex_basics_03 0)
  add_test(NAME basics_04
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/ex_04_simple_eval/build/tubex_basics_04 0)
  add_test(NAME basics_05
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/ex_05_graphics/build/tubex_basics_05 0)

  # Robotics
  add_test(NAME rob_01
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_01_causal_chain/build/tubex_rob_01 0)
  #add_test(NAME rob_02
  #         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_02_lowcost_beacons/build/tubex_rob_02 0)
  add_test(NAME rob_03
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_03_drifting_clock/build/tubex_rob_03 0)
  add_test(NAME rob_04
           COMMAND sh -c "rm -f ${CMAKE_CURRENT_SOURCE_DIR}/../data/redermor/gesmi.txt.tubex ;
                   ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_04_redermor_traj/build/tubex_rob_04 ${CMAKE_CURRENT_SOURCE_DIR}/../data/redermor/gesmi.txt 0")
  add_test(NAME rob_04_with_serialize
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_04_redermor_traj/build/tubex_rob_04 ${CMAKE_CURRENT_SOURCE_DIR}/../data/redermor/gesmi.txt 0)
  add_test(NAME rob_05
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_05_loops_detec/build/tubex_rob_05 ${CMAKE_CURRENT_SOURCE_DIR}/../data/redermor/gesmi.txt  0)
  add_test(NAME rob_06
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_06_loops_proofs/build/tubex_rob_06 0)
  add_test(NAME rob_07
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_07_dynloc/build/tubex_rob_07 0)
  add_test(NAME rob_08
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_08_tubepaving/build/tubex_rob_08 0)
  add_test(NAME rob_09
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_09_lissajous/build/tubex_rob_09 0)
  add_test(NAME rob_10
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/ex_10_datasso/build/tubex_rob_10 0)

  if(WITH_CAPD)
    # Lie group
    add_test(NAME lie_05
             COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lie_group/ex_05_loc/build/tubex_lie_05 0)
  endif()

endif()