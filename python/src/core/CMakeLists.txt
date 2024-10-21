# ==================================================================
#  codac / Python binding - cmake configuration file
# ==================================================================

# Generating a library core.so containing the python binding:

  pybind11_add_module(_core SHARED
    codac2_py_core.cpp
    
    actions/codac2_py_OctaSym.cpp
    
    contractors/codac2_py_Ctc.cpp
    contractors/codac2_py_Ctc.h
    contractors/codac2_py_CtcAction.cpp
    contractors/codac2_py_CtcCartProd.cpp
    contractors/codac2_py_CtcFixpoint.cpp
    contractors/codac2_py_CtcInnerOuter.cpp
    contractors/codac2_py_CtcInter.cpp
    contractors/codac2_py_CtcInverse.h
    contractors/codac2_py_CtcInverseNotIn.h
    contractors/codac2_py_CtcLazy.cpp
    contractors/codac2_py_CtcNot.cpp
    contractors/codac2_py_CtcPolar.cpp
    contractors/codac2_py_CtcProj.cpp
    contractors/codac2_py_CtcSegment.cpp
    contractors/codac2_py_CtcUnion.cpp
    contractors/codac2_py_CtcWrapper.cpp
    contractors/codac2_py_directed_ctc.cpp
    contractors/codac2_py_linear_ctc.cpp

    domains/interval/codac2_py_Interval.cpp
    domains/interval/codac2_py_Interval_operations.cpp
    domains/interval/codac2_py_IntervalMatrix.cpp
    domains/interval/codac2_py_IntervalMatrixBase.h
    domains/interval/codac2_py_IntervalVector.cpp
    domains/paving/codac2_py_Paving.cpp
    domains/paving/codac2_py_PavingNode.cpp
    domains/paving/codac2_py_Subpaving.cpp
    domains/codac2_py_BoolInterval.cpp

    functions/analytic/codac2_py_analytic_operations.cpp
    functions/analytic/codac2_py_analytic_variables.cpp
    functions/analytic/codac2_py_AnalyticFunction.h
    functions/analytic/codac2_py_ExprWrapper.h

    geometry/codac2_py_Edge.cpp
    geometry/codac2_py_geometry.cpp
    geometry/codac2_py_Polygon.cpp

    matrices/codac2_py_arithmetic_add.cpp
    matrices/codac2_py_arithmetic_sub.cpp
    matrices/codac2_py_arithmetic_mul.cpp
    matrices/codac2_py_arithmetic_div.cpp
    matrices/codac2_py_Matrix.cpp
    matrices/codac2_py_MatrixBase.h
    matrices/codac2_py_MatrixBaseBlock.h
    matrices/codac2_py_Vector.cpp
    matrices/codac2_py_VectorBase.h

    paver/codac2_py_pave.cpp

    separators/codac2_py_Sep.cpp
    separators/codac2_py_Sep.h
    separators/codac2_py_SepAction.cpp
    separators/codac2_py_SepCartProd.cpp
    separators/codac2_py_SepCtcBoundary.cpp
    separators/codac2_py_SepCtcPair.cpp
    separators/codac2_py_SepInter.cpp
    separators/codac2_py_SepInverse.h
    separators/codac2_py_SepNot.cpp
    separators/codac2_py_SepPolygon.cpp
    separators/codac2_py_SepProj.cpp
    separators/codac2_py_SepTransform.cpp
    separators/codac2_py_SepUnion.cpp
    separators/codac2_py_SepWrapper.cpp

    tools/codac2_py_Approx.cpp
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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/functions/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/graphics/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/matrices/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/paver/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/separators/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tools/
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