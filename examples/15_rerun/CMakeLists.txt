# ==================================================================
#  codac / rerun example - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.5)
project(codac_rerun_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CODAC REQUIRED)
message(STATUS "Found Codac version ${CODAC_VERSION}")

ibex_init_common()

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC ${CODAC_CXX_FLAGS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CODAC_LIBRARIES})
