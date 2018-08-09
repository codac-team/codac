# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 2.8.7)


# Settings:
  
  set(CMAKE_CXX_STANDARD 11)
  set(PACKAGE_NAME       tubex-lib)
  set(PACKAGE_VERSION    "2.0")
  project(${PACKAGE_NAME})
  get_filename_component(TUBELIB_BIN_DIR   "./bin"   ABSOLUTE)
  get_filename_component(TUBELIB_LIB_DIR   "./lib"   ABSOLUTE)
  get_filename_component(TUBELIB_DATA_DIR  "./data"  ABSOLUTE)
  get_filename_component(TUBELIB_BUILD_DIR "./make" ABSOLUTE)
  get_filename_component(TUBELIB_SRC_DIR   "./build" ABSOLUTE)
  set(DATA_DIRECTORY "${TUBELIB_DATA_DIR}" CACHE PATH "")
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/scripts/cmake_modules")


# Options

  option(BUILD_TESTS "Build test" ON)


# Position Independent Code

  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-fPIC SUPPORT_FPIC)

  if(SUPPORT_FPIC)
    set(CMAKE_CXX_FLAGS ${CMAKE_CSS_FLAGS} "-fPIC")
  endif(SUPPORT_FPIC)


# IBEX-lib package

  link_directories(/usr/local/lib/ibex/3rd/)
  include_directories(/usr/local/include/ibex/)
  include_directories(/usr/local/include/ibex/3rd/)
  #set(LIBS ${LIBS} -libex -lm -lgaol -lgdtoa -lultim) # using library Gaol
  set(LIBS ${LIBS} -libex -lm -lprim) # using library Filib


# To be removed:
  
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")


# Sources

  include_directories(${TUBELIB_BUILD_DIR}/include/)
  add_subdirectory(src)


# Tests

  #if(BUILD_TESTS)
  #  enable_testing(true)
  #  add_subdirectory(tests)
  #  include(CTest)
  #endif()


# Examples

  add_subdirectory(examples)


# Uninstallation

  configure_file("${CMAKE_HOME_DIRECTORY}/scripts/cmake_modules/cmake_uninstall.cmake.in" "cmake_uninstall.cmake" IMMEDIATE @ONLY)
  add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")