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
#include <codac2_ExprType.h>
#include <codac2_Row.h>
#include <codac2_IntervalRow.h>
#include <codac2_math.h>
#include <codac2_Sep.h>
#include "codac2_py_Sep.h"
#include "codac2_py_AnalyticFunction.h"
#include "codac2_py_CtcInverse.h"
#include "codac2_py_CtcInverseNotIn.h"
#include "codac2_py_MatrixBlock.h"
#include "codac2_py_Slice.h"
#include "codac2_py_SlicedTube.h"

using namespace codac2;
namespace py = pybind11;

// 3rd

// actions
void export_OctaSym(py::module& m);

// contractors
py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector> export_CtcIntervalVector(py::module& m);
void export_CtcAction(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcCartProd(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcCross(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcCtcBoundary(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcDist(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcEmpty(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcFixpoint(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcIdentity(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcInnerOuter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcInter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcLazy(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcNot(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcPointCloud(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
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
void export_TDomain(py::module& m);
void export_TSlice(py::module& m);
void export_TubeBase(py::module& m);

// functions
void export_ScalarVar(py::module& m);
void export_VectorVar(py::module& m);
void export_MatrixVar(py::module& m);

// geometry
void export_ConvexPolygon(py::module& m);
void export_Segment(py::module& m);
void export_geometry(py::module& m);
void export_Polygon(py::module& m);

// matrices
void export_cart_prod(py::module& m);
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
void export_hull(py::module& m);
void export_inversion(py::module& m);
void export_IntvFullPivLU(py::module& m);

// operators
void export_operators(py::module& m);

// paver
void export_pave(py::module& m);

// separators
py::class_<SepBase,pySep> export_Sep(py::module& m);
void export_SepAction(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepCartProd(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepChi(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepCross(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepCtcBoundary(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepCtcPair(py::module& m, py::class_<SepBase,pySep>& pysep);
void export_SepInter(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepInverse(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepNot(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepPolygon(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepProj(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepTransform(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepUnion(py::module& m, py::class_<SepBase,pySep>& sep);
void export_SepWrapper(py::module& m, py::class_<SepBase,pySep>& sep);

// tools
void export_Approx(py::module& m);
void export_transformations(py::module& m);
void export_RobotSimulator(py::module& m);

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
  export_CtcCross(m, py_ctc_iv);
  export_CtcCtcBoundary(m, py_ctc_iv);
  export_CtcDist(m, py_ctc_iv);
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
  export_CtcPointCloud(m, py_ctc_iv);
  export_CtcPolar(m, py_ctc_iv);
  export_CtcPolygon(m, py_ctc_iv);
  export_CtcProj(m, py_ctc_iv);
  export_CtcSegment(m, py_ctc_iv);
  export_CtcUnion(m, py_ctc_iv);
  export_CtcWrapper(m, py_ctc_iv);
  export_linear_ctc(m);

  // matrices
  export_cart_prod(m);
  py::class_<Row> exported_row_class(m, "Row", DOC_TO_BE_DEFINED);
  auto py_V = export_Vector(m);
  auto py_M = export_Matrix(m);
  auto py_B = export_EigenBlock<Matrix>(m, "MatrixBlock");
  export_EigenBlock<Row>(m, "RowBlock");
  export_EigenBlock<Vector>(m, "VectorBlock");
  export_hull(m);
  export_inversion(m);
  export_IntvFullPivLU(m);

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
  export_Slice<Interval>(m, "Slice_Interval");
  export_Slice<IntervalVector>(m, "Slice_IntervalVector");
  export_Slice<IntervalMatrix>(m, "Slice_IntervalMatrix");
  export_TDomain(m);
  export_TSlice(m);
  export_TubeBase(m);
  export_SlicedTube<Interval>(m, "SlicedTube_Interval");
  export_SlicedTube<IntervalVector>(m, "SlicedTube_IntervalVector");
  export_SlicedTube<IntervalMatrix>(m, "SlicedTube_IntervalMatrix");

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

  #if FOR_MATLAB // Python enums do not seem to be callable in matlab
  m.attr("EvalMode_NATURAL") = EvalMode::NATURAL;
  m.attr("EvalMode_CENTERED") = EvalMode::CENTERED;
  m.attr("EvalMode_DEFAULT") = EvalMode::DEFAULT;
  #endif

  export_ScalarExpr(m);
  export_VectorExpr(m);
  export_MatrixExpr(m);
  export_AnalyticFunction<ScalarType>(m,"AnalyticFunction_Scalar");
  export_AnalyticFunction<VectorType>(m,"AnalyticFunction_Vector");
  export_AnalyticFunction<MatrixType>(m,"AnalyticFunction_Matrix");
  export_ScalarVar(m);
  export_VectorVar(m);
  export_MatrixVar(m);

  // geometry
  export_Segment(m);
  export_geometry(m);
  export_Polygon(m);
  export_ConvexPolygon(m);

  // opearators
  export_operators(m);

  // paver
  export_pave(m);

  // separators
  auto py_sep = export_Sep(m);
  export_SepAction(m,py_sep);
  export_SepCartProd(m,py_sep);
  export_SepChi(m,py_sep);
  export_SepCross(m,py_sep);
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
  export_transformations(m);
  export_RobotSimulator(m);

  // trajectory
  export_AnalyticTraj(m);
  export_SampledTraj(m);
}
