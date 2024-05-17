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
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_analytic_variables.h>
#include <codac2_AnalyticExpr.h>
#include "codac2_py_analytic_variables_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_ExprWrapper.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_ScalarVar(py::module& m)
{
  py::class_<ScalarVar, std::shared_ptr<ScalarVar> /* due to enable_shared_from_this */>
    exported(m, "ScalarVar", SCALARVAR_MAIN);
  exported
  
    .def(py::init<>(),
      SCALARVAR_SCALARVAR)

    .def("size", &ScalarVar::size,
      SIZET_SCALARVAR_SIZE_CONST)

    .def("__add__", [](const ScalarVar& e1, const ScalarVar& e2) { return ExprWrapper<ScalarOpValue>(e1 + e2); })
    .def("__sub__", [](const ScalarVar& e1, const ScalarVar& e2) { return ExprWrapper<ScalarOpValue>(e1 - e2); })
    .def("__mul__", [](const ScalarVar& e1, const ScalarVar& e2) { return ExprWrapper<ScalarOpValue>(e1 * e2); })
    .def("__div__", [](const ScalarVar& e1, const ScalarVar& e2) { return ExprWrapper<ScalarOpValue>(e1 / e2); })
  ;

  py::implicitly_convertible<ScalarVar,ExprWrapper<ScalarOpValue>>();
}

void export_VectorVar(py::module& m)
{
  py::class_<VectorVar,
    std::shared_ptr<VectorVar> /* due to enable_shared_from_this */>
    exported(m, "VectorVar", VECTORVAR_MAIN);
  exported
  
    .def(py::init<size_t>(),
      VECTORVAR_VECTORVAR_SIZET)

    .def("size", &VectorVar::size,
      SIZET_VECTORVAR_SIZE_CONST)

    .def("__getitem__", [](const VectorVar& v, size_t index) -> ExprWrapper<ScalarOpValue>
      {
        if(index < 0 || index >= static_cast<size_t>(v.size()))
          throw py::index_error();
        return ExprWrapper<ScalarOpValue>(std::dynamic_pointer_cast<AnalyticExpr<ScalarOpValue>>(v[static_cast<int>(index)]->copy()));
      }, SHARED_PTR_ANALYTICEXPR_SCALAROPVALUE_VECTORVAR_OPERATORCOMPO_SIZET_CONST)

    .def("__add__", [](const VectorVar& e1, const VectorVar& e2) { return ExprWrapper<VectorOpValue>(e1 + e2); })
    .def("__sub__", [](const VectorVar& e1, const VectorVar& e2) { return ExprWrapper<VectorOpValue>(e1 - e2); })
    .def("__mul__", [](const ScalarVar& e1, const VectorVar& e2) { return ExprWrapper<VectorOpValue>(e1 * e2); })
  ;

  py::implicitly_convertible<VectorVar,ExprWrapper<VectorOpValue>>();
}