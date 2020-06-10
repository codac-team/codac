# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.0.2)

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/CMakeModules/)
  include(version_from_git)
  version_from_git() # Obtains the version number from Git tags

  # To use a specific compiler:
  #set(CMAKE_C_COMPILER "gcc-7")
  #set(CMAKE_CXX_COMPILER "/usr/bin/g++-7")

  project(tubex-lib VERSION ${VERSION} LANGUAGES CXX)

  if(NOT VERSION_ID)
    set(PROJECT_VERSION_FULL ${PROJECT_VERSION})
  else()
    set(PROJECT_VERSION_FULL "${PROJECT_VERSION}-${VERSION_ID}")
  endif()
  message(STATUS "Full project version is ${PROJECT_VERSION_FULL}")

  set(PROJECT_DESCRIPTION
      "Tubex is a library providing tools for constraint programming over reals and trajectories.")
  set(PROJECT_LONG_DESCRIPTION
      "${PROJECT_DESCRIPTION}")
  set(PROJECT_HOMEPAGE_URL "http://simon-rohou.fr/research/tubex-lib")
  message(STATUS "Configuring build for ${PROJECT_NAME} ${PROJECT_VERSION}")

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "Configuring ${PROJECT_NAME} in DEBUG mode as none was specified.")
  endif()

  
################################################################################
# Options for directories
################################################################################
  
  # Install directories
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
# Tests
################################################################################
  
  # Note: place this tests block before the add_subdirectory(python),
  # otherwise python tests will not be taken into account. 
  option(BUILD_TESTS "Build test" OFF)
  if(BUILD_TESTS)
    include(CTest)
    add_custom_target(check
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure $(ARGS)
                      DEPENDS tubex COMMENT "Running the tests")

    add_subdirectory(tests)
  endif()

  option(TEST_EXAMPLES "Testing examples" OFF)
  add_subdirectory(examples) # examples are tested as integration tests


################################################################################
# Compile sources
################################################################################

  add_compile_options(-fPIC) # is it necessary for python binding?
  add_subdirectory(src) # C++ sources
  add_subdirectory(doc) # documentation (Doxygen + Sphinx manual)

  # Python binding:
  if(WITH_PYTHON)
    add_subdirectory(python)
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