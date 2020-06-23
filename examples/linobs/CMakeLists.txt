# ==================================================================
#  tubex-lib / basics example - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.0.2)
  project(tubex_linobs LANGUAGES CXX)

# Compilation options

  set(CMAKE_CXX_STANDARD 17)
  add_compile_options(-O3 -Wall)
  #add_compile_options(-DNDEBUG) # comment for debug mode

# Adding IBEX

  # In case you installed IBEX in a local directory, you need 
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/ibex-lib/build_install")

  find_package(IBEX REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION}")

# Adding Tubex

  # In case you installed Tubex in a local directory, you need 
  # to specify its path with the CMAKE_PREFIX_PATH option.
  # set(CMAKE_PREFIX_PATH "~/tubex-lib/build_install")

  find_package(TUBEX REQUIRED)
  message(STATUS "Found Tubex version ${TUBEX_VERSION}")

# Adding Eigen

  find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Compilation

  add_executable(${PROJECT_NAME} main.cpp)
  target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${TUBEX_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${TUBEX_LIBRARIES} Ibex::ibex ${TUBEX_LIBRARIES} Eigen3::Eigen)