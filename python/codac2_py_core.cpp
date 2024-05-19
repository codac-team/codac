/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <codac2_Interval.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_analytic_values.h>
#include "codac2_py_AnalyticFunction.h"

using namespace codac2;
namespace py = pybind11;

// 3rd

// domains
py::class_<Interval> export_Interval(py::module& m);
void export_Interval_operations(py::module& m, py::class_<Interval>& py_Interval);
py::class_<IntervalVector> export_IntervalVector(py::module& m);
void export_IntervalVector_operations(py::module& m, py::class_<IntervalVector>& py_IntervalVector);

// functions
void export_ScalarVar(py::module& m);
void export_VectorVar(py::module& m);
void export_expression_operations(py::module& m);

// tools
void export_Approx(py::module& m);

PYBIND11_MODULE(core, m)
{
  m.doc() = "Python binding of Codac (core)";
  m.attr("oo") = oo;

  // 3rd

  // domains
  auto py_Interval = export_Interval(m);
  export_Interval_operations(m, py_Interval);
  auto py_IntervalVector = export_IntervalVector(m);
  export_IntervalVector_operations(m, py_IntervalVector);

  // function
  export_ScalarExpr(m);
  export_VectorExpr(m);
  export_AnalyticFunction<ScalarOpValue>(m,"ScalarAnalyticFunction");
  export_AnalyticFunction<VectorOpValue>(m,"VectorAnalyticFunction");
  export_ScalarVar(m);
  export_VectorVar(m);
  export_expression_operations(m);

  // tools
  export_Approx(m);

}