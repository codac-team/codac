# ==================================================================
#  tubex-lib / example - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER g++-5)
set(CMAKE_CXX_STANDARD 11)

set(TUBEX_ROOT ../../../)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../cmake/modules")
find_package(TubexLib REQUIRED)
find_package(IbexLib REQUIRED)

include_directories("${TUBEX_INCLUDE_DIR}" "${IBEX_INCLUDE_DIR}")

add_executable(solver_01_picard main.cpp)
target_link_libraries(solver_01_picard ${IBEX_LIBRARY} ${TUBEX_LIBRARY} ${IBEX_LIBRARY})