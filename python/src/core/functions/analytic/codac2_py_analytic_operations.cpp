/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_analytic_variables.h>
#include <codac2_analytic_operations.h>
#include <codac2_AnalyticFunction.h>
#include "codac2_py_analytic_operations_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_ExprWrapper.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_expression_operations(py::module& m)
{
  m .def("pow", [](const ExprWrapper<ScalarOpValue>& e1, const ExprWrapper<ScalarOpValue>& e2) { return ExprWrapper<ScalarOpValue>(pow(e1,e2)); },
      SCALAREXPR_PTR_POW_CONST_SCALAREXPR_PTR_REF_CONST_SCALAREXPR_PTR_REF,
      "x1"_a, "x2"_a)

    .def("pow", [](const Interval& e1, const ExprWrapper<ScalarOpValue>& e2) { return ExprWrapper<ScalarOpValue>(pow(e1,e2)); },
      SCALAREXPR_PTR_POW_CONST_X1_REF_CONST_SCALAREXPR_PTR_REF,
      "x1"_a, "x2"_a)

    .def("pow", [](const ExprWrapper<ScalarOpValue>& e1, const Interval& e2) { return ExprWrapper<ScalarOpValue>(pow(e1,e2)); },
      SCALAREXPR_PTR_POW_CONST_SCALAREXPR_PTR_REF_CONST_X2_REF,
      "x1"_a, "x2"_a)

    .def("sqr", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(sqr(e)); },
      SCALAREXPR_PTR_SQR_CONST_SCALAREXPR_PTR_REF,
      "x1"_a)

    .def("sqrt", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(sqrt(e)); },
      SCALAREXPR_PTR_SQRT_CONST_SCALAREXPR_PTR_REF,
      "x1"_a)

    .def("exp", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(exp(e)); },
      SCALAREXPR_PTR_EXP_CONST_SCALAREXPR_PTR_REF,
      "x1"_a)

    .def("cos", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(cos(e)); },
      SCALAREXPR_PTR_COS_CONST_SCALAREXPR_PTR_REF,
      "x1"_a)

    .def("sin", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(sin(e)); },
      SCALAREXPR_PTR_SIN_CONST_SCALAREXPR_PTR_REF,
      "x1"_a)

    .def("tanh", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(tanh(e)); },
      SCALAREXPR_PTR_TANH_CONST_SCALAREXPR_PTR_REF,
      "x1"_a)

    .def("abs", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(abs(e)); },
      SCALAREXPR_PTR_ABS_CONST_SCALAREXPR_PTR_REF,
      "x1"_a)

  ;

  const char* EVAL_VEC_DOC = VECTOREXPR_PTR_VEC_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC;
  using S = const ExprWrapper<ScalarOpValue>&;

  m .def("vec", [](S e1) { return ExprWrapper<VectorOpValue>(vec(e1)); }, EVAL_VEC_DOC, "x1"_a)
    .def("vec", [](S e1, S e2) { return ExprWrapper<VectorOpValue>(vec(e1,e2)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a)
    .def("vec", [](S e1, S e2, S e3) { return ExprWrapper<VectorOpValue>(vec(e1,e2,e3)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a)
    .def("vec", [](S e1, S e2, S e3, S e4) { return ExprWrapper<VectorOpValue>(vec(e1,e2,e3,e4)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5) { return ExprWrapper<VectorOpValue>(vec(e1,e2,e3,e4,e5)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6) { return ExprWrapper<VectorOpValue>(vec(e1,e2,e3,e4,e5,e6)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7) { return ExprWrapper<VectorOpValue>(vec(e1,e2,e3,e4,e5,e6,e7)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8) { return ExprWrapper<VectorOpValue>(vec(e1,e2,e3,e4,e5,e6,e7,e8)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8, S e9) { return ExprWrapper<VectorOpValue>(vec(e1,e2,e3,e4,e5,e6,e7,e8,e9)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a,"x9"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8, S e9, S e10) { return ExprWrapper<VectorOpValue>(vec(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a,"x9"_a,"x10"_a)
  ;
}