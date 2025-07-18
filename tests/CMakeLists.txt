# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

# Finding Catch2

  #message(STATUS "Getting Catch2 library... (an Internet connection might be required)")

  Include(FetchContent)

  #find_package(Catch2 3.6.0 EXACT QUIET)
  find_package(Catch2 3.6.0 QUIET)
  if(Catch2_FOUND)
    message(STATUS "Found Catch2 version ${Catch2_VERSION} in ${Catch2_DIR}")
  else()
    message(STATUS "Attempt to install Catch2 automatically...")
    FetchContent_Declare(
    Catch2
    #GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    #GIT_TAG        v3.6.0
    #URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.6.0.zip
    URL ${CMAKE_SOURCE_DIR}/3rd/Catch2-3.6.0.zip
    #SOURCE_DIR "P:/devel/GitHub/Catch2"
    #FIND_PACKAGE_ARGS
  )
  FetchContent_MakeAvailable(Catch2)
  # Adds Catch2::Catch2WithMain
  endif()

list(APPEND SRC_TESTS # listing files without extension

  # 3rd

  # Core

  core/3rd/codac2_tests_eigen

  core/actions/codac2_tests_OctaSym

  core/contractors/codac2_tests_CtcAction
  core/contractors/codac2_tests_CtcCartProd
  core/contractors/codac2_tests_CtcCtcBoundary
  core/contractors/codac2_tests_CtcFixpoint
  core/contractors/codac2_tests_CtcInter
  core/contractors/codac2_tests_CtcInverse
  core/contractors/codac2_tests_CtcInverseNotIn
  core/contractors/codac2_tests_CtcLazy
  core/contractors/codac2_tests_CtcPolygon
  core/contractors/codac2_tests_CtcSegment
  core/contractors/codac2_tests_linear_ctc
  ../doc/manual/manual/contractors/geometric/src

  core/domains/codac2_tests_BoolInterval
  core/domains/ellipsoid/codac2_tests_Ellipsoid
  core/domains/interval/codac2_tests_Interval
  core/domains/interval/codac2_tests_Interval_operations
  core/domains/interval/codac2_tests_IntervalMatrix
  core/domains/interval/codac2_tests_IntervalVector
  core/domains/tube/codac2_tests_TDomain
  core/domains/tube/codac2_tests_SlicedTube
  core/domains/tube/codac2_tests_SlicedTube_integral
  ../doc/manual/manual/intervals/src

  core/functions/analytic/codac2_tests_AnalyticFunction
  ../doc/manual/manual/functions/analytic/src

  core/geometry/codac2_tests_ConvexPolygon
  core/geometry/codac2_tests_geometry
  core/geometry/codac2_tests_Polygon
  core/geometry/codac2_tests_Segment
  ../doc/manual/manual/geometry/src

  core/matrices/codac2_tests_arithmetic_add
  core/matrices/codac2_tests_arithmetic_div
  core/matrices/codac2_tests_arithmetic_mul
  core/matrices/codac2_tests_arithmetic_sub
  core/matrices/codac2_tests_cart_prod
  core/matrices/codac2_tests_Matrix
  core/matrices/codac2_tests_Vector
  core/matrices/codac2_tests_hull
  core/matrices/codac2_tests_inversion
  core/matrices/codac2_tests_IntFullPivLU
  ../doc/manual/manual/linear/src

  core/operators/codac2_tests_operators

  core/separators/codac2_tests_SepCartProd
  core/separators/codac2_tests_SepCtcBoundary
  core/separators/codac2_tests_SepInverse
  core/separators/codac2_tests_SepPolygon
  core/separators/codac2_tests_SepProj
  core/separators/codac2_tests_SepTransform
  
  core/tools/codac2_tests_Approx
  core/tools/codac2_tests_transformations
  
  core/trajectory/codac2_tests_AnalyticTraj
  core/trajectory/codac2_tests_SampledTraj

  graphics/styles/codac2_tests_Color
)

file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/core/domains/tube/codac2_tests_predefined_tubes.py
  # Add here other Python files that are not tests but need to be exported to Python package
  # ...
  
  DESTINATION
  ${CMAKE_BINARY_DIR}/python/python_package/codac/tests
)

set(CODAC_LIBRARIES ${PROJECT_NAME}-core ${PROJECT_NAME}-graphics)

# CAPD test
if (WITH_CAPD)
  list(APPEND SRC_TESTS
    extensions/capd/codac2_tests_capd
    ../doc/manual/manual/extensions/capd/src
  )
  set(CODAC_LIBRARIES ${CODAC_LIBRARIES} ${PROJECT_NAME}-capd capd::capd)
endif()

# IBEX test
#if (WITH_IBEX)
  list(APPEND SRC_TESTS
    extensions/ibex/codac2_tests_ibex
  )
  set(CODAC_LIBRARIES ${CODAC_LIBRARIES} ${PROJECT_NAME}-ibex)
#endif()

foreach(SRC_TEST ${SRC_TESTS})
  string(REPLACE "/" "_" TEST_NAME ${SRC_TEST})
  string(REPLACE "codac2_tests_" "" TEST_NAME ${TEST_NAME})
  set(TEST_NAME codac2_tests_${TEST_NAME})

  # C++ test
  add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.cpp)
  set(CODAC_HEADERS_DIR ${CMAKE_CURRENT_BINARY_DIR}/../include)
  target_include_directories(${TEST_NAME} SYSTEM PUBLIC ${CODAC_HEADERS_DIR})
  target_link_libraries(${TEST_NAME} PUBLIC Ibex::ibex ${CODAC_LIBRARIES} PRIVATE Catch2::Catch2WithMain)
  add_dependencies(check ${TEST_NAME})
  add_test(NAME ${TEST_NAME}_cpp COMMAND ${TEST_NAME})

  # Python test
  if(WITH_PYTHON)
    add_test(NAME ${TEST_NAME}_py COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.py)
  endif()

endforeach()