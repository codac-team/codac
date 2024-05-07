# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

find_package(Catch2 3 REQUIRED)
message(STATUS "Found Catch2 version ${Catch2_VERSION}")

list(APPEND SRC_TESTS # listing files without extension

  # 3rd

  # Core
  core/3rd/codac2_tests_eigen
  core/3rd/codac2_tests_ibex
  core/domains/codac2_tests_Domain
  core/domains/interval/codac2_tests_Interval
  core/domains/interval/codac2_tests_Interval_bwd
  core/domains/interval/codac2_tests_Interval_operations
  core/domains/interval/codac2_tests_IntervalMatrix
  core/domains/interval/codac2_tests_IntervalVector
  core/functions/analytic/codac2_tests_AnalyticFunction
  core/tools/codac2_tests_Approx


)

foreach(SRC_TEST ${SRC_TESTS})
  string(REPLACE "/" "_" TEST_NAME ${SRC_TEST})
  string(REPLACE "codac2_tests_" "" TEST_NAME ${TEST_NAME})
  set(TEST_NAME codac2_tests_${TEST_NAME})

  # C++ test
  add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.cpp)
  set(CODAC_HEADERS_DIR ${CMAKE_CURRENT_BINARY_DIR}/../include)
  target_include_directories(${TEST_NAME} SYSTEM PUBLIC ${CODAC_HEADERS_DIR})
  target_link_libraries(${TEST_NAME} PUBLIC Ibex::ibex codac-core PRIVATE Catch2::Catch2WithMain)
  add_dependencies(check ${TEST_NAME})
  add_test(NAME ${TEST_NAME}_cpp COMMAND ${TEST_NAME})

  # Python test
  if(WITH_PYTHON)
    add_test(NAME ${TEST_NAME}_py COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.py)
  endif()

endforeach()