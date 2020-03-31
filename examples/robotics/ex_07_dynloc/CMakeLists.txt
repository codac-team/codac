# ==================================================================
#  tubex-lib / robotics example - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.0.2)
project(tubex_rob_07 LANGUAGES CXX)

# Compilation options
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-O3 -Wall)
#add_compile_options(-DNDEBUG) # comment for debug mode
if(WIN32)
  add_definitions(-U__STRICT_ANSI__)
endif()

# Dependencies
include(FindPkgConfig)
pkg_search_module(TUBEX REQUIRED tubex)
message(STATUS "Found Tubex version ${TUBEX_VERSION}")

# Compilation
add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC ${TUBEX_CFLAGS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${TUBEX_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${TUBEX_LDFLAGS})