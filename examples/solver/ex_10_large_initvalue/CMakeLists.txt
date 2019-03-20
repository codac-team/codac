# ==================================================================
#  tubex-lib / example - cmake configuration file
# ==================================================================

# Settings

  cmake_minimum_required(VERSION 3.0)
  set(CMAKE_CXX_COMPILER g++-5)
  set(CMAKE_CXX_STANDARD 11)
  #set(CMAKE_CXX_FLAGS ${CMAKE_CSS_FLAGS} "-DNDEBUG") # comment for debug


# Dependencies

  get_filename_component(TUBEX_ROOT "../../../" ABSOLUTE) # used for find_packages modules
  list(APPEND CMAKE_MODULE_PATH "${TUBEX_ROOT}/cmake/modules") # location of find_package modules
  find_package(TubexLib REQUIRED)
  find_package(IbexLib REQUIRED)
  include_directories("${TUBEX_INCLUDE_DIR}" "${IBEX_INCLUDE_DIR}")


# Example sources

  add_executable(solver_10_large_initvalue main.cpp)
  target_link_libraries(solver_10_large_initvalue ${IBEX_LIBRARY} ${TUBEX_LIBRARY} ${IBEX_LIBRARY})