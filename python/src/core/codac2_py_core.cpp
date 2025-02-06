/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <codac2_Interval.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_ValueType.h>
#include <codac2_Row.h>
#include <codac2_IntervalRow.h>
#include <codac2_math.h>
#include "codac2_py_AnalyticFunction.h"
#include "codac2_py_CtcInverse.h"
#include "codac2_py_CtcInverseNotIn.h"
#include "codac2_py_SepInverse.h"
#include "codac2_py_MatrixBlock.h"

using namespace codac2;
namespace py = pybind11;

// 3rd

// actions
void export_OctaSym(py::module& m);

// contractors
py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector> export_CtcIntervalVector(py::module& m);
void export_CtcAction(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcCartProd(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcCtcBoundary(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcEmpty(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcFixpoint(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcIdentity(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcInnerOuter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcInter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcLazy(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcNot(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcPolar(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcPolygon(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcProj(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcSegment(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcUnion(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcWrapper(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_linear_ctc(py::module& m);

// domains
void export_BoolInterval(py::module& m);
void export_Ellipsoid(py::module& m);
void export_Ellipsoid_utils(py::module& m);
py::class_<Interval> export_Interval(py::module& m);
void export_Interval_operations(py::module& m, py::class_<Interval>& py_Interval);
py::class_<IntervalRow> export_IntervalRow(py::module& m);
py::class_<IntervalVector> export_IntervalVector(py::module& m);
py::class_<IntervalMatrix> export_IntervalMatrix(py::module& m);
void export_Paving(py::module& m);
void export_PavingNode(py::module& m);
void export_Subpaving(py::module& m);

// functions
void export_ScalarVar(py::module& m);
void export_VectorVar(py::module& m);

// geometry
void export_Edge(py::module& m);
void export_geometry(py::module& m);
void export_Polygon(py::module& m);

// matrices
void export_arithmetic_add(
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<Eigen::Block<Matrix>>& py_B, py::class_<Eigen::Block<IntervalMatrix>>& py_IB);
void export_arithmetic_sub(
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<Eigen::Block<Matrix>>& py_B, py::class_<Eigen::Block<IntervalMatrix>>& py_IB);
void export_arithmetic_mul(
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<Eigen::Block<Matrix>>& py_B, py::class_<Eigen::Block<IntervalMatrix>>& py_IB);
void export_arithmetic_div(
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<Eigen::Block<Matrix>>& py_B, py::class_<Eigen::Block<IntervalMatrix>>& py_IB);
py::class_<Row> export_Row(py::module& m);
py::class_<Vector> export_Vector(py::module& m);
py::class_<Matrix> export_Matrix(py::module& m);
void export_Inversion(py::module& m);

// operators
void export_operators(py::module& m);

// paver
void export_pave(py::module& m);

// separators
py::class_<SepBase,pySep> export_Sep(py::module& m);
void export_SepAction(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepCartProd(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepCtcBoundary(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepCtcPair(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepInter(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepNot(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepPolygon(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepProj(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepTransform(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepUnion(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepWrapper(py::module& m, py::class_<SepBase,pySep>& sep);

// tools
void export_Approx(py::module& m);

// trajectory
void export_AnalyticTraj(py::module& m);
void export_SampledTraj(py::module& m);


PYBIND11_MODULE(_core, m)
{
  m.doc() = string(FOR_MATLAB ? "Matlab" : "Python") + " binding of Codac (core)";
  m.attr("oo") = oo;
  m.attr("PI") = PI;

  // 3rd

  // actions
  export_OctaSym(m);

  // contractors
  auto py_ctc_iv = export_CtcIntervalVector(m);
  export_CtcAction(m, py_ctc_iv);
  export_CtcCartProd(m, py_ctc_iv);
  export_CtcCtcBoundary(m, py_ctc_iv);
  export_CtcEmpty(m, py_ctc_iv);
  export_CtcFixpoint(m, py_ctc_iv);
  export_CtcIdentity(m, py_ctc_iv);
  export_CtcInnerOuter(m, py_ctc_iv);
  export_CtcInter(m, py_ctc_iv);
  export_CtcInverse<ScalarType>(m,"CtcInverse_Interval",py_ctc_iv);
  export_CtcInverse<VectorType>(m,"CtcInverse_IntervalVector",py_ctc_iv);
  export_CtcInverseNotIn<ScalarType>(m,"CtcInverseNotIn_Interval",py_ctc_iv);
  export_CtcInverseNotIn<VectorType>(m,"CtcInverseNotIn_IntervalVector",py_ctc_iv);
  export_CtcLazy(m, py_ctc_iv);
  export_CtcNot(m, py_ctc_iv);
  export_CtcPolar(m, py_ctc_iv);
  export_CtcPolygon(m, py_ctc_iv);
  export_CtcProj(m, py_ctc_iv);
  export_CtcSegment(m, py_ctc_iv);
  export_CtcUnion(m, py_ctc_iv);
  export_CtcWrapper(m, py_ctc_iv);
  export_linear_ctc(m);

  // matrices
  py::class_<Row> exported_row_class(m, "Row", DOC_TO_BE_DEFINED);
  auto py_V = export_Vector(m);
  auto py_M = export_Matrix(m);
  auto py_B = export_EigenBlock<Matrix>(m, "MatrixBlock");
  export_EigenBlock<Row>(m, "RowBlock");
  export_EigenBlock<Vector>(m, "VectorBlock");
  export_Inversion(m);

  // domains
  export_BoolInterval(m);
  export_Ellipsoid(m);
  export_Ellipsoid_utils(m);
  auto py_Interval = export_Interval(m);
  export_Interval_operations(m, py_Interval);
  auto py_IR = export_IntervalRow(m);
  auto py_IV = export_IntervalVector(m);
  auto py_IM = export_IntervalMatrix(m);
  auto py_IB = export_EigenBlock<IntervalMatrix>(m, "IntervalMatrixBlock");
  export_EigenBlock<IntervalRow>(m, "IntervalRowBlock");
  export_EigenBlock<IntervalVector>(m, "IntervalVectorBlock");

  export_arithmetic_add(py_V, py_IV, py_M, py_IM, py_B, py_IB);
  export_arithmetic_sub(py_V, py_IV, py_M, py_IM, py_B, py_IB);
  export_arithmetic_mul(py_V, py_IV, py_M, py_IM, py_B, py_IB);
  export_arithmetic_div(py_V, py_IV, py_M, py_IM, py_B, py_IB);
  
  export_Paving(m);
  export_PavingNode(m);
  export_Subpaving(m);

  // function
  py::enum_<EvalMode>(m, "EvalMode")
    .value("NATURAL", EvalMode::NATURAL)
    .value("CENTERED", EvalMode::CENTERED)
    .value("DEFAULT", EvalMode::DEFAULT)
    .def(py::self | py::self, EVALMODE_OPERATOROR_EVALMODE_EVALMODE)
  ;

  export_ScalarExpr(m);
  export_VectorExpr(m);
  export_AnalyticFunction<ScalarType>(m,"AnalyticFunction_Scalar");
  export_AnalyticFunction<VectorType>(m,"AnalyticFunction_Vector");
  export_ScalarVar(m);
  export_VectorVar(m);

  // geometry
  export_Edge(m);
  export_geometry(m);
  export_Polygon(m);

  // opearators
  export_operators(m);

  // paver
  export_pave(m);

  // separators
  auto py_sep = export_Sep(m);
  export_SepAction(m,py_sep);
  export_SepCartProd(m,py_sep);
  export_SepCtcBoundary(m,py_sep);
  export_SepCtcPair(m,py_sep);
  export_SepInter(m,py_sep);
  export_SepInverse(m,py_sep);
  export_SepNot(m,py_sep);
  export_SepPolygon(m,py_sep);
  export_SepProj(m,py_sep);
  export_SepTransform(m,py_sep);
  export_SepUnion(m,py_sep);
  export_SepWrapper(m,py_sep);

  // tools
  export_Approx(m);

  // trajectory
  export_AnalyticTraj(m);
  export_SampledTraj(m);
}
