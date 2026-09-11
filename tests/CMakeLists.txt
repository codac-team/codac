# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

# Finding Catch2

option(ENABLE_FIND_PACKAGE_CATCH2 "ENABLE_FIND_PACKAGE_CATCH2" ON)
if(ENABLE_FIND_PACKAGE_CATCH2)
  find_package(Catch2 3.0.0 QUIET) # Use Catch2_DIR or CMAKE_PREFIX_PATH to specify a custom Catch2 installation path...
endif()
if(Catch2_FOUND)
  message(STATUS "Found Catch2 version ${Catch2_VERSION} in ${Catch2_DIR}")
else()
  message(STATUS "Attempt to install Catch2 automatically...")
  FetchContent_Declare(Catch2 URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.6.0.zip)
  #FetchContent_Declare(Catch2 URL ${CMAKE_CURRENT_SOURCE_DIR}/../3rd/Catch2-3.6.0.zip) # If needed to be self-contained...
  FetchContent_MakeAvailable(Catch2)
  # See codac_mark_target_system() in the top-level CMakeLists.txt: without
  # this, Catch2's headers inherit this project's warning level. Only the
  # FetchContent path needs it -- a find_package() target is IMPORTED, and
  # CMake already treats those as system.
  codac_mark_target_system(Catch2)
  codac_mark_target_system(Catch2::Catch2WithMain)
endif()
# Adds Catch2::Catch2WithMain


# Test sources
# ==================================================================

list(APPEND SRC_TESTS # listing files without extension

  # ----------------------------------------------------------------
  # 3rd
  # ----------------------------------------------------------------

  core/3rd/codac2_tests_eigen


  # ----------------------------------------------------------------
  # Actions
  # ----------------------------------------------------------------

  core/actions/codac2_tests_OctaSym


  # ----------------------------------------------------------------
  # Contractors
  # ----------------------------------------------------------------

  core/contractors/codac2_tests_CtcAction
  core/contractors/codac2_tests_CtcCartProd
  core/contractors/codac2_tests_CtcCtcBoundary
  core/contractors/codac2_tests_CtcDeriv
  core/contractors/codac2_tests_CtcEval
  core/contractors/codac2_tests_CtcFixpoint
  core/contractors/codac2_tests_CtcInter
  core/contractors/codac2_tests_CtcInverse
  core/contractors/codac2_tests_CtcInverseNotIn
  core/contractors/codac2_tests_CtcLazy
  core/contractors/codac2_tests_CtcLohner
  core/contractors/codac2_tests_CtcPolygon
  core/contractors/codac2_tests_CtcQInter
  core/contractors/codac2_tests_CtcSegment
  core/contractors/codac2_tests_CtcUnion
  core/contractors/codac2_tests_CtcVisible
  core/contractors/codac2_tests_linear_ctc

  ../doc/manual/manual/contractors/geometric/src
  ../doc/manual/manual/contractors/analytic/src
  ../doc/manual/manual/contractors/set/src
  ../doc/manual/manual/contractors/shape/src


  # ----------------------------------------------------------------
  # Domains
  # ----------------------------------------------------------------

  core/domains/codac2_tests_BoolInterval

  core/domains/ellipsoid/codac2_tests_Ellipsoid

  core/domains/interval/codac2_tests_Interval
  core/domains/interval/codac2_tests_Interval_operations
  core/domains/interval/codac2_tests_IntervalMatrix
  core/domains/interval/codac2_tests_IntervalVector

  ../doc/manual/manual/intervals/src

  core/domains/zonotope/codac2_tests_Parallelepiped_eval
  core/domains/zonotope/codac2_tests_Parallelepiped
  core/domains/zonotope/codac2_tests_Zonotope

  core/domains/tube/codac2_tests_TDomain
  core/domains/tube/codac2_tests_Slice
  core/domains/tube/codac2_tests_Slice_polygon
  core/domains/tube/codac2_tests_SlicedTube
  core/domains/tube/codac2_tests_SlicedTube_integral

  ../doc/manual/manual/tubes/src


  # ----------------------------------------------------------------
  # Functions
  # ----------------------------------------------------------------

  core/functions/analytic/codac2_tests_AnalyticFunction

  ../doc/manual/manual/functions/analytic/src
  ../doc/manual/manual/functions/parallelepiped/src


  # ----------------------------------------------------------------
  # Geometry
  # ----------------------------------------------------------------

  core/geometry/codac2_tests_ConvexPolygon
  core/geometry/codac2_tests_geometry
  core/geometry/codac2_tests_Polygon
  core/geometry/codac2_tests_Segment

  ../doc/manual/manual/geometry/src


  # ----------------------------------------------------------------
  # Matrices
  # ----------------------------------------------------------------

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
  core/matrices/codac2_tests_GaussJordan

  ../doc/manual/manual/linear/src


  # ----------------------------------------------------------------
  # Operators
  # ----------------------------------------------------------------

  core/operators/codac2_tests_operators


  # ----------------------------------------------------------------
  # Peibos
  # ----------------------------------------------------------------

  core/peibos/codac2_tests_peibos

  ../doc/manual/manual/functions/peibos/src


  # ----------------------------------------------------------------
  # Separators
  # ----------------------------------------------------------------

  core/separators/codac2_tests_SepCartProd
  core/separators/codac2_tests_SepCtcBoundary
  core/separators/codac2_tests_SepInter
  core/separators/codac2_tests_SepInverse
  core/separators/codac2_tests_SepPolygon
  core/separators/codac2_tests_SepProj
  core/separators/codac2_tests_SepQInter
  core/separators/codac2_tests_SepTransform
  core/separators/codac2_tests_SepUnion
  core/separators/codac2_tests_SepVisible


  # ----------------------------------------------------------------
  # Tools
  # ----------------------------------------------------------------

  core/tools/codac2_tests_Approx
  core/tools/codac2_tests_serialization
  core/tools/codac2_tests_transformations
  core/tools/codac2_tests_trunc
  core/tools/ibex/codac2_tests_ibex

  ../doc/manual/manual/tools/src


  # ----------------------------------------------------------------
  # Trajectory
  # ----------------------------------------------------------------

  core/trajectory/codac2_tests_AnalyticTraj
  core/trajectory/codac2_tests_SampledTraj


  # ----------------------------------------------------------------
  # Graphics
  # ----------------------------------------------------------------

  graphics/styles/codac2_tests_Color
)


# Python files required by tests
# ==================================================================

file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/core/domains/tube/codac2_tests_predefined_tubes.py
  # Add here other Python files that are not tests but need to be exported to Python package
  # ...
  
  DESTINATION
  ${CMAKE_BINARY_DIR}/python/python_package/codac/tests
)


# Libraries
# ==================================================================

set(CODAC_LIBRARIES ${PROJECT_NAME}-core ${PROJECT_NAME}-graphics)

# Sympy
# ==================================================================

option(BUILD_SYMPY_EMBED_TESTS "Build C++ tests that require embedded Python for Sympy" ON)
if(WITH_PYTHON AND DEFINED PYBIND11_FINDPYTHON AND NOT PYBIND11_FINDPYTHON)
  message(STATUS "Disabling Sympy C++ embed tests because PYBIND11_FINDPYTHON=OFF")
  set(BUILD_SYMPY_EMBED_TESTS OFF)
endif()

# Sympy test
if (WITH_PYTHON AND BUILD_SYMPY_EMBED_TESTS)
  list(APPEND SRC_TESTS
    extensions/sympy/codac2_tests_sympy
    ../doc/manual/manual/extensions/sympy/src
  )
endif()


# CAPD
# ==================================================================
if (WITH_CAPD)
  list(APPEND SRC_TESTS
    extensions/capd/codac2_tests_capd
    extensions/capd/codac2_tests_peibos_capd
    ../doc/manual/manual/extensions/capd/src
  )
  set(CODAC_LIBRARIES ${CODAC_LIBRARIES} ${PROJECT_NAME}-capd capd::capd)
endif()

# IBEX test
#if (WITH_IBEX)
  #find_package(IBEX REQUIRED)
  #ibex_init_common() # IBEX should have installed this function
  #message(STATUS "Found IBEX version ${IBEX_VERSION}")
#endif()

foreach(SRC_TEST ${SRC_TESTS})
  string(REPLACE "/" "_" TEST_NAME ${SRC_TEST})
  string(REPLACE "codac2_tests_" "" TEST_NAME ${TEST_NAME})
  set(TEST_NAME codac2_tests_${TEST_NAME})

  # C++ test
  add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.cpp)

  set_target_properties(${TEST_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  set(CODAC_HEADERS_DIR ${CMAKE_CURRENT_BINARY_DIR}/../include)
  target_include_directories(${TEST_NAME} SYSTEM PUBLIC ${CODAC_HEADERS_DIR})
  target_link_libraries(${TEST_NAME} PUBLIC Ibex::ibex ${CODAC_LIBRARIES} PRIVATE Catch2::Catch2WithMain)

  # Propagate the architecture flags (FMA, etc.) to the tests
  if(CODAC_ARCH_CXX_FLAGS)
    target_compile_options(${TEST_NAME} PRIVATE ${CODAC_ARCH_CXX_FLAGS})
  endif()

  if(WITH_PYTHON AND SRC_TEST MATCHES "extensions/sympy/")
    target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME}-sympy pybind11::embed)
  endif()
  add_dependencies(check ${TEST_NAME})
  add_test(NAME ${TEST_NAME}_cpp COMMAND ${TEST_NAME})

  # Python test
  if(WITH_PYTHON)
    add_test(NAME ${TEST_NAME}_py COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.py)
  endif()

endforeach()
