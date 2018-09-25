# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Settings:

  cmake_minimum_required(VERSION 2.8.7)

  set(CMAKE_CXX_STANDARD 11)
  set(PACKAGE_NAME       tubex-lib)
  set(PACKAGE_VERSION    "2.0")
  set(CMAKE_MODULE_PATH  "${PROJECT_SOURCE_DIR}/scripts/cmake_modules")

  project(${PACKAGE_NAME})

  get_filename_component(TUBELIB_BIN_DIR   "./bin"   ABSOLUTE)
  get_filename_component(TUBELIB_LIB_DIR   "./lib"   ABSOLUTE)
  get_filename_component(TUBELIB_BUILD_DIR "./make" ABSOLUTE)
  get_filename_component(TUBELIB_SRC_DIR   "./build" ABSOLUTE)

  #set(CMAKE_CXX_FLAGS ${CMAKE_CSS_FLAGS} "-DNDEBUG") # Comment for debug

# IBEX-lib package

  link_directories(/usr/local/lib/ibex/3rd/)
  include_directories(/usr/local/include/ibex/)
  include_directories(/usr/local/include/ibex/3rd/)
  #set(LIBS ${LIBS} -libex -lm -lgaol -lgdtoa -lultim) # using library Gaol
  set(LIBS ${LIBS} -libex -lm -lprim) # using library Filib

# Sources

  include_directories(${TUBELIB_BUILD_DIR}/include/)
  add_subdirectory(src)

# Tests

  option(BUILD_TESTS "Build test" ON)
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