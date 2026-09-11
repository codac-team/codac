# ==================================================================
#  codac / Python binding - cmake configuration file
# ==================================================================

# Generating a library core.so containing the python binding:

  pybind11_add_module(_core SHARED
    codac2_py_core.cpp
    codac2_py_cast.h
    
    actions/codac2_py_OctaSym.cpp
    
    contractors/codac2_py_Ctc.cpp
    contractors/codac2_py_Ctc.h
    contractors/codac2_py_CtcAction.cpp
    contractors/codac2_py_CtcCartProd.cpp
    contractors/codac2_py_CtcConstell.cpp
    contractors/codac2_py_CtcCross.cpp
    contractors/codac2_py_CtcCtcBoundary.cpp
    contractors/codac2_py_CtcDeriv.cpp
    contractors/codac2_py_CtcDist.cpp
    contractors/codac2_py_CtcEmpty.cpp
    contractors/codac2_py_CtcEval.cpp
    contractors/codac2_py_CtcFixpoint.cpp
    contractors/codac2_py_CtcInnerOuter.cpp
    contractors/codac2_py_CtcIdentity.cpp
    contractors/codac2_py_CtcInter.cpp
    contractors/codac2_py_CtcInverse.h
    contractors/codac2_py_CtcLazy.cpp
    contractors/codac2_py_CtcLohner.cpp
    contractors/codac2_py_CtcNot.cpp
    contractors/codac2_py_CtcPointCloud.cpp
    contractors/codac2_py_CtcPolar.cpp
    contractors/codac2_py_CtcPolygon.cpp
    contractors/codac2_py_CtcProj.cpp
    contractors/codac2_py_CtcQInter.cpp
    contractors/codac2_py_CtcSegment.cpp
    contractors/codac2_py_CtcUnion.cpp
    contractors/codac2_py_CtcVisible.cpp
    contractors/codac2_py_CtcWrapper.cpp
    contractors/codac2_py_linear_ctc.cpp

    domains/ellipsoid/codac2_py_Ellipsoid.cpp
    domains/ellipsoid/codac2_py_Ellipsoid_utils.cpp
    domains/interval/codac2_py_Interval.cpp
    domains/interval/codac2_py_Interval_operations.cpp
    domains/interval/codac2_py_IntervalMatrix.cpp
    domains/interval/codac2_py_IntervalMatrixBase.h
    domains/interval/codac2_py_IntervalRow.cpp
    domains/interval/codac2_py_IntervalVector.cpp
    domains/interval/codac2_py_IntervalVector_templ.h
    domains/paving/codac2_py_Paving.cpp
    domains/paving/codac2_py_PavingNode.cpp
    domains/paving/codac2_py_Subpaving.cpp
    domains/zonotope/codac2_py_Parallelepiped.cpp
    domains/zonotope/codac2_py_Zonotope.cpp
    domains/tube/codac2_py_Slice.h
    domains/tube/codac2_py_SlicedTube.h
    domains/tube/codac2_py_SlicedTube_operations.cpp
    domains/tube/codac2_py_TDomain.cpp
    domains/tube/codac2_py_TimePropag.cpp
    domains/tube/codac2_py_TSlice.cpp
    domains/tube/codac2_py_TubeBase.cpp
    domains/tube/codac2_py_tube_cart_prod.cpp
    domains/codac2_py_BoolInterval.cpp

    extensions/sympy/codac2_py_sympy.cpp

    functions/analytic/codac2_py_analytic_variables.cpp
    functions/analytic/codac2_py_AnalyticFunction.h
    functions/analytic/codac2_py_AnalyticExprWrapper.h

    geometry/codac2_py_ConvexPolygon.cpp
    geometry/codac2_py_geometry.cpp
    geometry/codac2_py_Polygon.cpp
    geometry/codac2_py_Segment.cpp

    matrices/codac2_py_cart_prod.cpp
    matrices/codac2_py_arithmetic_add.cpp
    matrices/codac2_py_arithmetic_sub.cpp
    matrices/codac2_py_arithmetic_mul.cpp
    matrices/codac2_py_arithmetic_div.cpp
    matrices/codac2_py_hull.cpp
    matrices/codac2_py_inversion.cpp
    matrices/codac2_py_IntvFullPivLU.cpp
    matrices/codac2_py_GaussJordan.cpp
    matrices/codac2_py_Matrix.cpp
    matrices/codac2_py_MatrixBase.h
    matrices/codac2_py_MatrixBlock.h
    matrices/codac2_py_Vector.cpp
    matrices/codac2_py_VectorBase.h

    operators/codac2_py_operators.cpp

    paver/codac2_py_pave.cpp

    peibos/codac2_py_peibos.cpp

    proj/codac2_py_qinter.cpp

    separators/codac2_py_Sep.cpp
    separators/codac2_py_Sep.h
    separators/codac2_py_SepAction.cpp
    separators/codac2_py_SepCartPolar.cpp
    separators/codac2_py_SepCartProd.cpp
    separators/codac2_py_SepChi.cpp
    separators/codac2_py_SepCross.cpp
    separators/codac2_py_SepCtcBoundary.cpp
    separators/codac2_py_SepCtcPair.cpp
    separators/codac2_py_SepInter.cpp
    separators/codac2_py_SepInverse.cpp
    separators/codac2_py_SepNot.cpp
    separators/codac2_py_SepPolarCart.cpp
    separators/codac2_py_SepPolygon.cpp
    separators/codac2_py_SepProj.cpp
    separators/codac2_py_SepQInter.cpp
    separators/codac2_py_SepTransform.cpp
    separators/codac2_py_SepUnion.cpp
    separators/codac2_py_SepVisible.cpp
    separators/codac2_py_SepWrapper.cpp

    tools/codac2_py_Approx.cpp
    tools/codac2_py_RobotSimulator.cpp
    tools/codac2_py_serialization.cpp
    tools/codac2_py_threading.cpp
    tools/codac2_py_transformations.cpp
    tools/codac2_py_trunc.cpp

    trajectory/codac2_py_AnalyticTraj.cpp
    trajectory/codac2_py_SampledTraj.cpp
    trajectory/codac2_py_TrajBase.h
    trajectory/codac2_py_traj_cart_prod.cpp
  )

  target_include_directories(_core
    # First, so that a header is opened at src/... rather than through
    # build/include: the path a translation unit used is the path gcov records,
    # and the two spellings would split every header into two entries in the
    # coverage report. See codac_publish_include_dirs() in the top-level CMakeLists.txt.
    PRIVATE ${CODAC_SOURCE_INCLUDE_DIRS}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../docstring
    PRIVATE ${CMAKE_BINARY_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/actions/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/contractors/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/zonotope/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extensions/sympy/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/functions/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/graphics/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/matrices/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/paver/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/peibos/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/separators/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tools/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/
  )

  target_link_libraries(_core
    PRIVATE ${PROJECT_NAME}-core ${PROJECT_NAME}-sympy Threads::Threads Ibex::ibex
  )

  # Propager les flags d'architecture (FMA, etc.) vers les modules Python (Option 2)
  if(CODAC_ARCH_CXX_FLAGS)
    target_compile_options(_core PRIVATE ${CODAC_ARCH_CXX_FLAGS})
  endif()

  # --------------------------------------------------------------
  # Sanitizer runtime as a shared library (Debug builds only)
  # --------------------------------------------------------------
  #
  # _core.so is loaded via dlopen() by an unsanitized python
  # interpreter. By default Clang/GCC embed the ASan/UBSan runtime
  # statically into shared libraries, which leaves symbols like
  # __ubsan_vptr_type_cache or __asan_* unresolved at dlopen() time
  # (python has no sanitizer runtime of its own to provide them).
  # -shared-libsan makes _core.so depend on the sanitizer runtime as
  # a proper shared library instead; that runtime is then LD_PRELOAD-ed
  # for the python tests (see tests/CMakeLists.txt).
  # Not gated behind CMAKE_BUILD_TYPE (see the matching note in the root
  # CMakeLists.txt): that variable is meaningless for multi-configuration
  # generators, so -shared-libsan is applied via a $<CONFIG:Debug>
  # generator expression instead, which CMake evaluates correctly for
  # both single- and multi-config generators.
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(_core PRIVATE $<$<CONFIG:Debug>:-shared-libsan>)
    target_link_options(_core PRIVATE $<$<CONFIG:Debug>:-shared-libsan>)
  endif()

  # --------------------------------------------------------------
  # MSVC AddressSanitizer runtime DLL (Debug builds only)
  # --------------------------------------------------------------
  #
  # /fsanitize=address (enabled in Debug mode only if a matching runtime
  # was found -- see the MSVC branch in the root CMakeLists.txt, which
  # sets CODAC_MSVC_ASAN_RUNTIME_DLL) makes _core.pyd import that runtime
  # DLL. Unlike Clang/GCC on Linux, there is no LD_PRELOAD-style env var
  # and no "must be the first library loaded" requirement to work around
  # on Windows -- the loader simply needs to be able to find that DLL when
  # Python calls LoadLibrary() on _core.pyd, which it does by searching
  # the directory of the loading module before PATH. So it is enough to
  # copy the runtime DLL next to the built _core.pyd, the same way
  # _core.pyd itself is already copied into the package folder below.
  # CODAC_MSVC_ASAN_RUNTIME_DLL is already empty (and this is then a
  # no-op) whenever the root CMakeLists.txt couldn't locate that runtime,
  # so /fsanitize=address was in that case never even added.
  if(MSVC AND CODAC_MSVC_ASAN_RUNTIME_DLL)
    # CODAC_MSVC_ASAN_RUNTIME_DLL is now found regardless of which
    # configuration will be built (see the root CMakeLists.txt), since a
    # multi-config generator does not know that yet at configure time. So
    # the copy itself has to stay conditional on the configuration that is
    # actually being built, which a plain if() cannot express:
    # $<IF:$<CONFIG:Debug>,copy_if_different,true> selects between the
    # real "-E copy_if_different" and cmake's "-E true" (a documented
    # no-op that ignores its extra arguments) so the DLL is only copied
    # for a Debug-configuration build, in any generator.
    add_custom_command(TARGET _core POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,copy_if_different,true> "${CODAC_MSVC_ASAN_RUNTIME_DLL}" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
    )
  endif()


  # Copy the generated library in the package folder
  add_custom_command(TARGET _core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_core>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
  )

  # ex: from codac import core
  #     core.Tube
  # ex: from codac.core import Tube
  #     Tube
