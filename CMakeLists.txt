# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.0.2)

  project(TUBEX VERSION 2.5.0 LANGUAGES CXX)
  set(TUBEX_DESCRIPTION "Tubex is a library providing tools for constraint "
                        "programming over reals and trajectories.")
  set(TUBEX_URL "http://simon-rohou.fr/research/tubex-lib")
  message(STATUS ${TUBEX_DESCRIPTION})

  set(CMAKE_BUILD_TYPE Debug)

  
################################################################################
# Options for install directory
################################################################################

  set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
  set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
  set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
  set(CMAKE_INSTALL_PKGCONFIG "share/pkgconfig" CACHE PATH "pkg files (share/pkgconfig)")
  set(CMAKE_INSTALL_CMAKE "share/tubex/cmake" CACHE PATH "cmake files (share/tubex/cmake)")


################################################################################
# Compilation configuration
################################################################################

  # Check that the compiler supports c++11
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)

  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    message(FATAL_ERROR "Tubex needs a compiler with C++11 support")
  endif()

  if(WIN32)
    # We need this for strdup under Windows (see issue #287 of ibex-lib repo)
    add_definitions(-U__STRICT_ANSI__)
  endif()

  add_compile_options(-O3 -Wall)

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
  endif()


################################################################################
# Optional binary tree activation for all tubes (for tests purposes mainly)
################################################################################
  
  # Binary trees allow to speed up some computations. They can be activated
  # upon request according to the application. For tests purposes, the following
  # forces the use of binary trees for all computations.
  option(WITH_TUBE_TREE "Binary trees for fast tube evaluations" OFF)

  if(WITH_TUBE_TREE)
    message(STATUS "[binary trees] Using binary trees for fast tube evaluations")
    add_definitions(-DUSE_TUBE_TREE)
  endif()


################################################################################
# Looking for IBEX
################################################################################
  
  find_package(IBEX REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION}")


################################################################################
# Looking for CAPD (if needed)
################################################################################

  option(WITH_CAPD "Using CAPD for accurate integration of ODEs" OFF)

  if(WITH_CAPD)
    # This looks for capd.pc file
    include(FindPkgConfig)
    pkg_search_module(PKG_CAPD REQUIRED capd capd-gui mpcapd mpcapd-gui)
    include_directories(${PKG_CAPD_INCLUDE_DIRS})
    message(STATUS "[capd2tubex] PKG_CAPD_INCLUDE_DIRS = ${PKG_CAPD_INCLUDE_DIRS}")
    message(STATUS "[capd2tubex] PKG_CAPD_LDFLAGS = ${PKG_CAPD_LDFLAGS}")
  endif()


################################################################################
# Python binding
################################################################################

  add_compile_options(-fPIC) # is it necessary for python binding?
  #add_subdirectory(pybind11)


################################################################################
# Compile sources
################################################################################

  add_subdirectory(src)
  #add_subdirectory(python)


################################################################################
# Tests
################################################################################

  option(BUILD_TESTS "Build test" OFF)
  if(BUILD_TESTS)

    include(CTest)
    add_custom_target(check
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure $(ARGS)
                      DEPENDS tubex COMMENT "Running the tests")
    add_subdirectory(tests)

    # Testing examples:

      # Basics
      add_test(NAME basics_01
               COMMAND ../examples/basics/ex_01_arithmetic/build/tubex_basics_01 0)
      add_test(NAME basics_02
               COMMAND ../examples/basics/ex_02_simple_ctc/build/tubex_basics_02 0)
      add_test(NAME basics_03
               COMMAND ../examples/basics/ex_03_ivp_msinx/build/tubex_basics_03 0)
      add_test(NAME basics_04
               COMMAND ../examples/basics/ex_04_simple_eval/build/tubex_basics_04 0)
      add_test(NAME basics_05
               COMMAND ../examples/basics/ex_05_graphics/build/tubex_basics_05 0)

      # Robotics
      add_test(NAME rob_01
               COMMAND ../examples/robotics/ex_01_causal_chain/build/tubex_rob_01 0)
      add_test(NAME rob_02
               COMMAND ../examples/robotics/ex_02_lowcost_beacons/build/tubex_rob_02 0)
      add_test(NAME rob_03
               COMMAND ../examples/robotics/ex_03_drifting_clock/build/tubex_rob_03 0)
      add_test(NAME rob_04
               COMMAND sh -c "rm -f ../data/redermor/gesmi.txt.tubex ;
                       ../examples/robotics/ex_04_redermor_traj/build/tubex_rob_04 ../data/redermor/gesmi.txt 0")
      add_test(NAME rob_04_with_serializ
               COMMAND ../examples/robotics/ex_04_redermor_traj/build/tubex_rob_04 ../data/redermor/gesmi.txt 0)
      add_test(NAME rob_05
               COMMAND ../examples/robotics/ex_05_loops_detec/build/tubex_rob_05 ../data/redermor/gesmi.txt  0)
      add_test(NAME rob_06
               COMMAND ../examples/robotics/ex_06_loops_proofs/build/tubex_rob_06 0)
      add_test(NAME rob_07
               COMMAND ../examples/robotics/ex_07_dynloc/build/tubex_rob_07 0)
      add_test(NAME rob_08
               COMMAND ../examples/robotics/ex_08_tubepaving/build/tubex_rob_08 0)
      add_test(NAME rob_09
               COMMAND ../examples/robotics/ex_09_lissajous/build/tubex_rob_09 0)
      add_test(NAME rob_10
               COMMAND ../examples/robotics/ex_10_datasso/build/tubex_rob_10 0)

      # Tuto
      add_test(NAME tuto_01
               COMMAND ../examples/tuto/ex_01_getting_started/build/tubex_tuto_01 0)
      
  endif()


################################################################################
# Archives and packages
################################################################################

  set(CPACK_GENERATOR "TGZ" "ZIP" "DEB")
  string(TOLOWER "${CMAKE_PROJECT_NAME}" CPACK_PACKAGE_NAME)
  set(CPACK_PACKAGE_VENDOR "TubexTeam")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${TUBEX_DESCRIPTION})
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maintainer <simon.rohou@ensta-bretagne.fr>")
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${TUBEX_URL})
  # TODO finish deb package

  include(CPack)
