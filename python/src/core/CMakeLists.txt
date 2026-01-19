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
    contractors/codac2_py_CtcInverseNotIn.h
    contractors/codac2_py_CtcLazy.cpp
    contractors/codac2_py_CtcNot.cpp
    contractors/codac2_py_CtcPointCloud.cpp
    contractors/codac2_py_CtcPolar.cpp
    contractors/codac2_py_CtcPolygon.cpp
    contractors/codac2_py_CtcProj.cpp
    contractors/codac2_py_CtcQInter.cpp
    contractors/codac2_py_CtcSegment.cpp
    contractors/codac2_py_CtcUnion.cpp
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
    domains/tube/codac2_py_TDomain.cpp
    domains/tube/codac2_py_TimePropag.cpp
    domains/tube/codac2_py_TSlice.cpp
    domains/tube/codac2_py_TubeBase.cpp
    domains/tube/codac2_py_tube_cart_prod.cpp
    domains/codac2_py_BoolInterval.cpp

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
    separators/codac2_py_SepPolygon.cpp
    separators/codac2_py_SepProj.cpp
    separators/codac2_py_SepQInter.cpp
    separators/codac2_py_SepTransform.cpp
    separators/codac2_py_SepUnion.cpp
    separators/codac2_py_SepWrapper.cpp

    tools/codac2_py_Approx.cpp
    tools/codac2_py_RobotSimulator.cpp
    tools/codac2_py_serialization.cpp
    tools/codac2_py_transformations.cpp
    tools/codac2_py_trunc.cpp

    trajectory/codac2_py_AnalyticTraj.cpp
    trajectory/codac2_py_SampledTraj.cpp
    trajectory/codac2_py_TrajBase.h
  )

  target_include_directories(_core
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../docstring
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../../include 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/actions/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/contractors/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/zonotope/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/
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
    PRIVATE ${PROJECT_NAME}-core ${LIBS} Ibex::ibex
  )

  # Copy the generated library in the package folder
  add_custom_command(TARGET _core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:_core>" "${PYTHON_PACKAGE_DIR}/${PYTHON_PACKAGE_NAME}"
  )

  # ex: from codac import core
  #     core.Tube
  # ex: from codac.core import Tube
  #     Tube
