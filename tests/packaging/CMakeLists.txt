# ==================================================================
#  Codac - the find_package() half of the packaging check
# ==================================================================
#
# Built against an installed Codac, exactly the way a user's project is and the
# way every examples/*/CMakeLists.txt is: find_package(CODAC), then the three
# variables its config file publishes. check_consumer_parity.sh builds this,
# builds the same main.cpp from pkg-config alone, and compares the two.

  cmake_minimum_required(VERSION 3.14)
  project(codac_packaging_check LANGUAGES CXX)

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  find_package(CODAC REQUIRED)
  message(STATUS "Found Codac version ${CODAC_VERSION}")

  # As in the examples: IBEX contributes the interval-arithmetic flags
  # (-frounding-math and the rest). codac.pc carries the same set, which is
  # part of what the parity check compares.
  ibex_init_common()

  add_executable(${PROJECT_NAME} main.cpp)
  target_compile_options(${PROJECT_NAME} PUBLIC ${CODAC_CXX_FLAGS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${CODAC_LIBRARIES})
