# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

# Testing examples:

if(BUILD_TESTS AND TEST_EXAMPLES)

#  todo: compilation of the examples from CMake (remove ./make all alternative)

#  include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
#
#  list(APPEND CMAKE_ARGS "-DCODAC_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../src/")
#  list(APPEND CMAKE_ARGS "-DCODAC_LIBRARY=${CMAKE_CURRENT_SOURCE_DIR}/../src/core/libcodac.a")
#  list(APPEND CMAKE_ARGS "-DCODAC_ROB_LIBRARY=${CMAKE_CURRENT_SOURCE_DIR}/../src/robotics/libcodac-rob.a")
#  list(APPEND CMAKE_ARGS "-DCODAC_ODE_LIBRARY=${CMAKE_CURRENT_SOURCE_DIR}/../src/3rd/ode/libcodac-ode.a")
#  list(APPEND CMAKE_ARGS "-DCODAC_INCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../include/")
#  list(APPEND CMAKE_ARGS "-DCODAC_ROB_INCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../include/")
#  list(APPEND CMAKE_ARGS "-DCODAC_ODE_INCLUDE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../include/")
#
#  list(APPEND CODAC_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../src/core/libcodac.a")
#  list(APPEND CODAC_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../src/robotics/libcodac-rob.a")
#  list(APPEND CODAC_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../src/3rd/ode/libcodac-ode.a")
#
#  list(APPEND CMAKE_ARGS "-DCODAC_INCLUDE_DIRS=${CMAKE_CURRENT_SOURCE_DIR}/../include")
#  list(APPEND CMAKE_ARGS "-DCODAC_LIBRARIES=${CODAC_LIBRARIES}")
#
#  ExternalProject_Add(basics_01
#                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/basics/01_arithmetic
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
  #         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/01_arithmetic/build/codac_basics_01 0)
  add_test(NAME basics_02
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/02_simple_ctc/build/codac_basics_02 0)
  add_test(NAME basics_03
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/03_ivp_msinx/build/codac_basics_03 0)
  add_test(NAME basics_04
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/04_simple_eval/build/codac_basics_04 0)
  add_test(NAME basics_05
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/05_graphics/build/codac_basics_05 0)
  add_test(NAME basics_07
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/07_temporal_ctc/build/codac_basics_07 0)
  if(WITH_PYTHON)
  add_test(NAME basics_07_py
           COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/basics/07_temporal_ctc/07_temporal_ctc.py 0)
  endif()
  add_test(NAME basics_08
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/08_tube_paving/build/codac_basics_08 0)
  add_test(NAME basics_09
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/basics/09_cn_paving/build/codac_basics_09 0)

  # Robotics
  add_test(NAME rob_01
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/01_causal_chain/build/codac_rob_01 0)
  #add_test(NAME rob_02
  #         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/02_lowcost_beacons/build/codac_rob_02 0)
  add_test(NAME rob_03
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/03_drifting_clock/build/codac_rob_03 0)
  add_test(NAME rob_04
           COMMAND sh -c "rm -f ${CMAKE_CURRENT_SOURCE_DIR}/../data/redermor/gesmi.txt.codac ;
                   ${CMAKE_CURRENT_SOURCE_DIR}/robotics/04_redermor_traj/build/codac_rob_04 ${CMAKE_CURRENT_SOURCE_DIR}/../data/redermor/gesmi.txt 0")
  add_test(NAME rob_04_with_serialize
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/04_redermor_traj/build/codac_rob_04 ${CMAKE_CURRENT_SOURCE_DIR}/../data/redermor/gesmi.txt 0)
  add_test(NAME rob_05
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/05_loops_detec/build/codac_rob_05 ${CMAKE_CURRENT_SOURCE_DIR}/../data/redermor/gesmi.txt  0)
  add_test(NAME rob_06
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/06_loops_proofs/build/codac_rob_06 0)
  add_test(NAME rob_07
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/07_dynloc/build/codac_rob_07 0)
  add_test(NAME rob_08
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/08_tubepaving/build/codac_rob_08 0)
  add_test(NAME rob_09
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/09_lissajous/build/codac_rob_09 0)
  add_test(NAME rob_10
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/robotics/10_datasso/build/codac_rob_10 0)

  # Linobs
  add_test(NAME linobs
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/linobs/01_paper/build/codac_linobs 0)
  add_test(NAME linobs_foreach_dt
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/linobs/01_paper/build/codac_linobs_foreach_dt 0)

  if(WITH_CAPD)
    # Lie group
    add_test(NAME lie_05
             COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lie_group/05_loc/build/codac_lie_05 0)
  endif()

endif()