# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.14...3.25)

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/CMakeModules/)
  include(version_from_git)
  version_from_git() # Obtains the version number from Git tags

  # To use a specific compiler:
  #set(CMAKE_C_COMPILER "gcc-7")
  #set(CMAKE_CXX_COMPILER "/usr/bin/g++-7")

  project(codac VERSION ${VERSION} LANGUAGES CXX)

  if(NOT VERSION_ID)
    set(PROJECT_VERSION_FULL ${PROJECT_VERSION})
  else()
    set(PROJECT_VERSION_FULL "${PROJECT_VERSION}${VERSION_ID}")
  endif()
  message(STATUS "Full project version is ${PROJECT_VERSION_FULL}")

  set(PROJECT_DESCRIPTION
      "Codac is a library providing tools for constraint programming over reals and trajectories.")
  set(PROJECT_LONG_DESCRIPTION
      "${PROJECT_DESCRIPTION}")
  set(PROJECT_HOMEPAGE_URL "http://codac.io")
  message(STATUS "Configuring build for ${PROJECT_NAME} ${PROJECT_VERSION}")


################################################################################
# Options for directories
################################################################################
  
  # Install directories
  set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
  set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
  set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
  set(CMAKE_INSTALL_PKGCONFIG "share/pkgconfig" CACHE PATH "pkg files (share/pkgconfig)")
  set(CMAKE_INSTALL_CMAKE "share/codac/cmake" CACHE PATH "cmake files (share/codac/cmake)")


################################################################################
# Compilation configuration
################################################################################
  
  include(FetchContent)

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
  endif()
  
  if(FAST_RELEASE)
    add_compile_definitions(FAST_RELEASE)
  endif()
  
  if(NOT WIN32)
    string(ASCII 27 Esc)
    set(COLOR_RED "${Esc}[31m")
    set(COLOR_BLUE "${Esc}[36m")
    set(COLOR_RESET "${Esc}[m")
  else()
    set(COLOR_RED "")
    set(COLOR_BLUE "")
    set(COLOR_RESET "")
  endif()

  message(STATUS "${COLOR_BLUE}Configuring ${PROJECT_NAME} in ${CMAKE_BUILD_TYPE} mode.${COLOR_RESET}")

  if(FAST_RELEASE)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      message(STATUS "${COLOR_RED}Option FAST_RELEASE cannot be used in Debug mode.${COLOR_RESET}")
    else()
      message(STATUS "${COLOR_BLUE}Option FAST_RELEASE enabled.${COLOR_RESET}")
    endif()
  else()
    message(STATUS "Note: in Release build mode, the option FAST_RELEASE=ON would allow faster computations if enabled.")
  endif()

  if(MSVC)
    add_compile_options(/W4)
  else()
    add_compile_options(-Wall -Wextra -Wpedantic)
  endif()

  # Temporary attempts to fix errors similar to: 
  # _ number of sections exceeded object file format limit.
  # _ out of memory allocating XXX bytes.
  # _ string table overflow.
  # _ .obj: file too big.
  if(MSVC)
    add_compile_options(/bigobj)
  elseif(MINGW)
    add_compile_options(-Wa,-mbig-obj)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
      add_compile_options(-g0)
    endif()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      add_compile_options(-Os)
    endif()
  endif()

#  # Check that the compiler supports c++20
#  include(CheckCXXCompilerFlag)
#  check_cxx_compiler_flag("-std=c++2a" COMPILER_SUPPORTS_CXX20)
#  
#  if(COMPILER_SUPPORTS_CXX20)
#    add_compile_options(-std=c++2a)
#  else()
#    message(FATAL_ERROR "Codac needs a compiler with C++20 support")
#  endif()

  #if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
  #endif()


################################################################################
# Looking for IBEX
################################################################################

  find_package(IBEX REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION}")


################################################################################
# Looking for Eigen3
################################################################################

  option(ENABLE_FIND_PACKAGE_EIGEN3 "ENABLE_FIND_PACKAGE_EIGEN3" OFF)
  if(ENABLE_FIND_PACKAGE_EIGEN3)
    find_package(Eigen3 3.4.0 QUIET) # Use Eigen3_DIR or CMAKE_PREFIX_PATH to specify a custom Eigen3 installation path...
  endif()
  if(Eigen3_FOUND)
    message(STATUS "Found Eigen3 version ${Eigen3_VERSION} in ${Eigen3_DIR}")
  else()
    message(STATUS "Attempt to install custom Eigen3 automatically...")
    FetchContent_Declare(Eigen3 URL http://github.com/codac-team/eigen/archive/refs/heads/eigen-5.0.zip)
    #FetchContent_Declare(Eigen3 URL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/eigen-a0dc3994bd5d4e804dab58decb78c56a5af516ba.zip) # If needed to be self-contained...
    FetchContent_MakeAvailable(Eigen3)
  endif()
  # Adds Eigen3::Eigen

################################################################################
# Looking for Threads
################################################################################

  find_package(Threads REQUIRED)

################################################################################
# Looking for CAPD (if needed)
################################################################################

  option(WITH_CAPD "Using CAPD for accurate integration of ODEs" OFF)

  if(WITH_CAPD)
    find_package(CAPD REQUIRED)
  endif()

################################################################################
# Compile sources
################################################################################
  
  add_subdirectory(src) # C++ sources
  add_subdirectory(doc) # documentation (Doxygen + Sphinx manual)

  # Python binding:
  option(WITH_PYTHON "Build Python binding" OFF)
  if(WITH_PYTHON)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    add_subdirectory(python)
  endif()


################################################################################
# Tests
################################################################################
  
  # Note: place this tests block before the add_subdirectory(python),
  # otherwise python tests will not be taken into account. 
  option(BUILD_TESTS "Build test" OFF)
  if(${CMAKE_VERSION} VERSION_LESS "3.14.0")
    # Disable tests due to FetchContent_MakeAvailable which needs CMake >= 3.14.
    set(BUILD_TESTS OFF CACHE BOOL "Build test" FORCE)
  endif()
  if(BUILD_TESTS)
    include(CTest)
    add_custom_target(check
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure $(ARGS)
                      DEPENDS ${PROJECT_NAME} COMMENT "Running the tests")

    add_subdirectory(tests)
  endif()

  option(TEST_EXAMPLES "Testing examples" OFF)
  add_subdirectory(examples) # examples are tested as integration tests


################################################################################
# Archives and packages
################################################################################

  set(CPACK_GENERATOR "TGZ" "ZIP" "DEB")
  string(TOLOWER "${CMAKE_PROJECT_NAME}" CPACK_PACKAGE_NAME)
  set(CPACK_PACKAGE_VENDOR "Codac Team")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CODAC_DESCRIPTION})
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maintainer <simon.rohou@ensta.fr>")
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CODAC_URL})
  # todo: finish deb package

  include(CPack)