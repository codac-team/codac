# ==================================================================
#  tubex-lib / basics example - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.0.2)
  project(01_getting_started LANGUAGES CXX)

# Adding IBEX

  # In case you installed IBEX in a local directory, you need 
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/ibex-lib/build_install")

  find_package(IBEX REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION}")

# Adding Eigen3

  # In case you installed Eigen3 in a local directory, you need
  # to specify its path with the CMAKE_PREFIX_PATH option, e.g.
  # set(CMAKE_PREFIX_PATH "~/eigen/build_install")

  find_package(Eigen3 REQUIRED NO_MODULE)
  message(STATUS "Found Eigen3 version ${EIGEN3_VERSION}")

# Adding Tubex

  # In case you installed Tubex in a local directory, you need 
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/tubex-lib/build_install")

  find_package(TUBEX REQUIRED)
  message(STATUS "Found Tubex version ${TUBEX_VERSION}")

# Compilation

  add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
  target_compile_options(${PROJECT_NAME} PUBLIC ${TUBEX_CXX_FLAGS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${TUBEX_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${TUBEX_LIBRARIES} Ibex::ibex ${TUBEX_LIBRARIES})