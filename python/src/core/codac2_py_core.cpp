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
#include <codac2_analytic_values.h>
#include "codac2_py_AnalyticFunction.h"
#include "codac2_py_CtcInverse.h"
#include "codac2_py_CtcInverseNotIn.h"
#include "codac2_py_SepInverse.h"
#include "codac2_py_MatrixBaseBlock.h"

using namespace codac2;
namespace py = pybind11;

// 3rd

// actions
void export_OctaSym(py::module& m);

// contractors
//py::class_<Ctc,pyCtc> export_Ctc(py::module& m);
py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector> export_CtcIntervalVector(py::module& m/*, py::class_<Ctc,pyCtc>& py_ctc*/);
void export_CtcAction(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcCartProd(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcFixpoint(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcInnerOuter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcInter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcLazy(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcNot(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcPolar(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcProj(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcSegment(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcUnion(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcWrapper(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_directed_ctc(py::module& m);
void export_linear_ctc(py::module& m);

// domains
void export_BoolInterval(py::module& m);
py::class_<Interval> export_Interval(py::module& m);
void export_Interval_operations(py::module& m, py::class_<Interval>& py_Interval);
py::class_<IntervalVector> export_IntervalVector(py::module& m);
py::class_<IntervalMatrix> export_IntervalMatrix(py::module& m);
void export_Paving(py::module& m);
void export_PavingNode(py::module& m);
void export_Subpaving(py::module& m);

// functions
void export_ExprWrapperBase(py::module& m);
void export_ScalarVar(py::module& m);
void export_VectorVar(py::module& m);
void export_expression_operations(py::module& m);

// geometry
void export_Edge(py::module& m);
void export_geometry(py::module& m);
void export_Polygon(py::module& m);

// matrices
void export_arithmetic_add(py::module& m,
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<MatrixBaseBlock<EigenMatrix<double>&,double>>& py_B, py::class_<MatrixBaseBlock<EigenMatrix<Interval>&,Interval>>& py_IB);
void export_arithmetic_sub(py::module& m,
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<MatrixBaseBlock<EigenMatrix<double>&,double>>& py_B, py::class_<MatrixBaseBlock<EigenMatrix<Interval>&,Interval>>& py_IB);
void export_arithmetic_mul(py::module& m,
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<MatrixBaseBlock<EigenMatrix<double>&,double>>& py_B, py::class_<MatrixBaseBlock<EigenMatrix<Interval>&,Interval>>& py_IB);
void export_arithmetic_div(py::module& m,
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<MatrixBaseBlock<EigenMatrix<double>&,double>>& py_B, py::class_<MatrixBaseBlock<EigenMatrix<Interval>&,Interval>>& py_IB);
py::class_<Vector> export_Vector(py::module& m);
py::class_<Matrix> export_Matrix(py::module& m);

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


PYBIND11_MODULE(_core, m)
{
  m.doc() = string(FOR_MATLAB ? "Matlab" : "Python") + " binding of Codac (core)";
  m.attr("oo") = oo;

  // 3rd

  // actions
  export_OctaSym(m);

  // contractors
  //auto py_ctc = export_Ctc(m);
  auto py_ctc_iv = export_CtcIntervalVector(m/*,py_ctc*/);
  export_CtcAction(m, py_ctc_iv);
  export_CtcCartProd(m, py_ctc_iv);
  export_CtcFixpoint(m, py_ctc_iv);
  export_CtcInnerOuter(m, py_ctc_iv);
  export_CtcInter(m, py_ctc_iv);
  export_CtcInverse<Interval>(m,"CtcInverse_Interval",py_ctc_iv);
  export_CtcInverse<IntervalVector>(m,"CtcInverse_IntervalVector",py_ctc_iv);
  export_CtcInverseNotIn<Interval>(m,"CtcInverseNotIn_Interval",py_ctc_iv);
  export_CtcInverseNotIn<IntervalVector>(m,"CtcInverseNotIn_IntervalVector",py_ctc_iv);
  export_CtcLazy(m, py_ctc_iv);
  export_CtcNot(m, py_ctc_iv);
  export_CtcPolar(m, py_ctc_iv);
  export_CtcProj(m, py_ctc_iv);
  export_CtcSegment(m, py_ctc_iv);
  export_CtcUnion(m, py_ctc_iv);
  export_CtcWrapper(m, py_ctc_iv);
  export_directed_ctc(m);
  export_linear_ctc(m);

  // matrices
  auto py_M = export_Matrix(m);
  auto py_V = export_Vector(m);
  auto py_B = export_MatrixBaseBlock<Matrix,double>(m, "MatrixBaseBlock_Matrix_double");

  // domains
  export_BoolInterval(m);
  auto py_Interval = export_Interval(m);
  export_Interval_operations(m, py_Interval);
  auto py_IV = export_IntervalVector(m);
  auto py_IM = export_IntervalMatrix(m);
  auto py_IB = export_MatrixBaseBlock<IntervalMatrix,Interval>(m, "MatrixBaseBlock_IntervalMatrix_Interval");

  export_arithmetic_add(m, py_V, py_IV, py_M, py_IM, py_B, py_IB);
  export_arithmetic_sub(m, py_V, py_IV, py_M, py_IM, py_B, py_IB);
  export_arithmetic_mul(m, py_V, py_IV, py_M, py_IM, py_B, py_IB);
  export_arithmetic_div(m, py_V, py_IV, py_M, py_IM, py_B, py_IB);
  
  export_Paving(m);
  export_PavingNode(m);
  export_Subpaving(m);

  // function
  export_ExprWrapperBase(m);
  export_ScalarExpr(m);
  export_VectorExpr(m);
  export_AnalyticFunction<ScalarOpValue>(m,"AnalyticFunction_Scalar");
  export_AnalyticFunction<VectorOpValue>(m,"AnalyticFunction_Vector");
  export_ScalarVar(m);
  export_VectorVar(m);
  export_expression_operations(m);

  // geometry
  export_Edge(m);
  export_geometry(m);
  export_Polygon(m);

  // paver
  export_pave(m);

  // separators
  auto py_sep = export_Sep(m);
  export_SepAction(m,py_sep);
  export_SepCartProd(m,py_sep);
  export_SepCtcBoundary(m,py_sep);
  export_SepCtcPair(m,py_sep);
  export_SepInter(m,py_sep);
  export_SepInverse<Interval>(m,"SepInverse_Interval",py_sep);
  export_SepInverse<IntervalVector>(m,"SepInverse_IntervalVector",py_sep);
  export_SepNot(m,py_sep);
  export_SepPolygon(m,py_sep);
  export_SepProj(m,py_sep);
  export_SepTransform(m,py_sep);
  export_SepUnion(m,py_sep);
  export_SepWrapper(m,py_sep);

  // tools
  export_Approx(m);

}