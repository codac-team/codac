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
#include "codac2_py_core.h"

using namespace codac2;
namespace py = pybind11;

// 3rd

// actions
void export_OctaSym(py::module& m);

// contractors
//py::class_<Ctc,pyCtc> export_Ctc(py::module& m);
py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector> export_CtcIntervalVector(py::module& m/*, py::class_<Ctc,pyCtc>& py_ctc*/);
void export_CtcAction(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcCartProd(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcInter(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcLazy(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcUnion(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& ctc);
void export_CtcWrapper(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& ctc);

// domains
void export_BoolInterval(py::module& m);
py::class_<Interval> export_Interval(py::module& m);
void export_Interval_operations(py::module& m, py::class_<Interval>& py_Interval);
py::class_<IntervalVector> export_IntervalVector(py::module& m);
void export_IntervalVector_operations(py::module& m, py::class_<IntervalVector>& py_IntervalVector);
py::class_<IntervalMatrix> export_IntervalMatrix(py::module& m);
void export_IntervalMatrix_operations(py::module& m, py::class_<IntervalMatrix>& py_IntervalMatrix);

// functions
void export_ExprWrapperBase(py::module& m);
void export_ScalarVar(py::module& m);
void export_VectorVar(py::module& m);
void export_expression_operations(py::module& m);

// graphics
void export_Figure2D(py::module& m);
void export_StyleProperties(py::module& m);

// matrices
void export_Matrix(py::module& m);
void export_Vector(py::module& m);

// paver
void export_Paver(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& ctc);

// separators
py::class_<Sep,pySep> export_Sep(py::module& m);
void export_SepAction(py::module& m, py::class_<Sep,pySep>& pysep);
void export_SepCartProd(py::module& m, py::class_<Sep,pySep>& pysep);
void export_SepInter(py::module& m, py::class_<Sep,pySep>& sep);
void export_SepPolygon(py::module& m, py::class_<Sep,pySep>& sep);
void export_SepUnion(py::module& m, py::class_<Sep,pySep>& sep);
void export_SepWrapper(py::module& m, py::class_<Sep,pySep>& sep);

// tools
void export_Approx(py::module& m);


PYBIND11_MODULE(core, m)
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
  export_CtcInter(m, py_ctc_iv);
  export_CtcInverse<Interval>(m,"CtcInverse_Interval",py_ctc_iv);
  export_CtcInverse<IntervalVector>(m,"CtcInverse_IntervalVector",py_ctc_iv);
  export_CtcInverseNotIn<Interval>(m,"CtcInverseNotIn_Interval",py_ctc_iv);
  export_CtcInverseNotIn<IntervalVector>(m,"CtcInverseNotIn_IntervalVector",py_ctc_iv);
  export_CtcLazy(m, py_ctc_iv);
  export_CtcUnion(m, py_ctc_iv);
  export_CtcWrapper(m, py_ctc_iv);

  // matrices
  export_Matrix(m);
  export_Vector(m);

  // domains
  export_BoolInterval(m);
  auto py_Interval = export_Interval(m);
  export_Interval_operations(m, py_Interval);
  auto py_IntervalVector = export_IntervalVector(m);
  export_IntervalVector_operations(m, py_IntervalVector);
  auto py_IntervalMatrix = export_IntervalMatrix(m);
  export_IntervalMatrix_operations(m, py_IntervalMatrix);

  // function
  export_ExprWrapperBase(m);
  export_ScalarExpr(m);
  export_VectorExpr(m);
  export_AnalyticFunction<ScalarOpValue>(m,"AnalyticFunction_Scalar");
  export_AnalyticFunction<VectorOpValue>(m,"AnalyticFunction_Vector");
  export_ScalarVar(m);
  export_VectorVar(m);
  export_expression_operations(m);

  // graphics
  export_StyleProperties(m);
  export_Figure2D(m);

  // paver
  export_Paver(m,py_ctc_iv);

  // separators
  auto py_sep = export_Sep(m);
  export_SepAction(m,py_sep);
  export_SepCartProd(m,py_sep);
  export_SepInter(m,py_sep);
  export_SepInverse<Interval>(m,"SepInverse_Interval",py_sep);
  export_SepInverse<IntervalVector>(m,"SepInverse_IntervalVector",py_sep);
  export_SepPolygon(m,py_sep);
  export_SepUnion(m,py_sep);
  export_SepWrapper(m,py_sep);

  // tools
  export_Approx(m);

}