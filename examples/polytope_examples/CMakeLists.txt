# ==================================================================
#  codac / basics example - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.5)
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

# Adding Codac

# In case you installed Codac in a local directory, you need 
# to specify its path with the CMAKE_PREFIX_PATH option.
# set(CMAKE_PREFIX_PATH "~/codac/build_install")

find_package(CODAC REQUIRED)
message(STATUS "Found Codac version ${CODAC_VERSION}")

# Compilation

if(FAST_RELEASE)
  add_compile_definitions(FAST_RELEASE)
  message(STATUS "You are running Codac in fast release mode. (option -DCMAKE_BUILD_TYPE=Release is required)")
endif()

set(PROGS main-F2V main-3Dgraphics main-V2F main-3Dhull main-3Dtest main-manual main-contractor)

foreach(PROG ${PROGS})
  add_executable(${PROG} ${PROG}.cpp)
  target_compile_options(${PROG} PUBLIC ${CODAC_CXX_FLAGS})
  target_include_directories(${PROG} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS})
  target_link_libraries(${PROG} PUBLIC ${CODAC_LIBRARIES} Ibex::ibex)
endforeach()
