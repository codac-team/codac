# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 2.8.7)


# Settings:
  
  set(CMAKE_CXX_STANDARD 11)
  set(PACKAGE_NAME      tubex-lib)
  set(PACKAGE_VERSION   "0.1")
  project(${PACKAGE_NAME})
  get_filename_component(TUBELIB_BIN_DIR  "./bin"   ABSOLUTE)
  get_filename_component(TUBELIB_LIB_DIR  "./lib"   ABSOLUTE)
  get_filename_component(TUBELIB_DATA_DIR "./data"  ABSOLUTE)
  set(DATA_DIRECTORY "${TUBELIB_DATA_DIR}" CACHE PATH "")
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/scripts/cmake_modules")


# Options

  option(BUILD_TESTS "Build test" ON)
  option(ONLY_PYTHON_BINDINGS "build library with python bindings only" OFF)


# Position Independent Code

  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-fPIC SUPPORT_FPIC)
  if(SUPPORT_FPIC)
    set(CMAKE_CXX_FLAGS ${CMAKE_CSS_FLAGS} "-fPIC")
  endif(SUPPORT_FPIC)


# IBEX-lib package

  set(IbexLib_USE_STATIC 1) # force ibex static linking
  find_package(IbexLib)
  if(IBEX_FOUND)
    include_directories(${IBEX_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${IBEX_LIBRARIES})
  else()
    message(FATAL_ERROR "Unable to find IBEX-lib. You may need to set IBEX_ROOT ${IBEX_ROOT}")
  endif()


# OpenMP library

  #find_package(OpenMP)
  #if(OPENMP_FOUND)
  #    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  #endif()


# Sources

  include_directories(./build/include)
  add_subdirectory(src)


# Tests

  if(BUILD_TESTS)
    enable_testing(true)
    add_subdirectory(tests)
    include(CTest)
  endif()


# Examples

  add_subdirectory(examples)


# Uninstallation

  configure_file("${CMAKE_HOME_DIRECTORY}/scripts/cmake_modules/cmake_uninstall.cmake.in" "cmake_uninstall.cmake" IMMEDIATE @ONLY)
  add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")