# ==================================================================
#  codac / ellipsoid example - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.10)
  project(codac_example LANGUAGES CXX)

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adding IBEX

  # In case you installed IBEX in a local directory, you need 
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/ibex-lib/build_install")

  find_package(IBEX REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION}")

# Adding Eigen3

  # Codac builds and installs its own Eigen3 (see CODAC_INCLUDE_DIRS below),
  # so no separate find_package(Eigen3) is needed -- and none would find it,
  # since that build isn't registered as a system-discoverable package.
  # Searching for an unrelated system Eigen3 here would risk pulling in a
  # different version than the one codac-core/codac-graphics were compiled
  # against, which is exactly the kind of ABI mismatch that broke examples
  # like 01_batman (see the -mfma / CODAC_CXX_FLAGS fix above in this history).

# Adding Codac

  # In case you installed Codac in a local directory, you need
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/codac/build_install")

  find_package(CODAC REQUIRED)
  message(STATUS "Found Codac version ${CODAC_VERSION}")


# Compilation

  add_executable(${PROJECT_NAME} main.cpp)
  target_compile_options(${PROJECT_NAME} PUBLIC ${CODAC_CXX_FLAGS})
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${CODAC_LIBRARIES} Ibex::ibex)
