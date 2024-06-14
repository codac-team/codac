# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

# Finding Catch2

  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.6.0
  )

  FetchContent_MakeAvailable(Catch2)

list(APPEND SRC_TESTS # listing files without extension

  # 3rd

  # Core
  core/3rd/codac2_tests_eigen
  core/3rd/codac2_tests_ibex
  core/domains/codac2_tests_BoolInterval
  core/domains/codac2_tests_Domain
  core/domains/interval/codac2_tests_Interval
  core/domains/interval/codac2_tests_Interval_bwd
  core/domains/interval/codac2_tests_Interval_operations
  core/domains/interval/codac2_tests_IntervalMatrix
  core/domains/interval/codac2_tests_IntervalVector
  core/functions/analytic/codac2_tests_AnalyticFunction
  core/geometry/codac2_tests_geometry
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
  target_link_libraries(${TEST_NAME} PUBLIC Ibex::ibex ${PROJECT_NAME}-core PRIVATE Catch2::Catch2WithMain)
  add_dependencies(check ${TEST_NAME})
  add_test(NAME ${TEST_NAME}_cpp COMMAND ${TEST_NAME})

  # Python test
  if(WITH_PYTHON)
    add_test(NAME ${TEST_NAME}_py COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.py)
  endif()

endforeach()