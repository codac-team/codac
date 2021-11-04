# ==================================================================
#  codac / basics example - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.0.2)
  project(codac_basics_09 LANGUAGES CXX)

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

# Adding Codac

  # In case you installed Codac in a local directory, you need 
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/codac/build_install")

  find_package(CODAC REQUIRED)
  message(STATUS "Found Codac version ${CODAC_VERSION}")

# Compilation
  
  set(CMAKE_BUILD_TYPE Debug)
  add_executable(${PROJECT_NAME} main.cpp)
  target_compile_options(${PROJECT_NAME} PUBLIC ${CODAC_CXX_FLAGS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${CODAC_LIBRARIES} Ibex::ibex ${CODAC_LIBRARIES})