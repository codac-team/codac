/** 
 *  \file
 *  Function Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_analytic_variables.h>
#include <codac2_analytic_operations.h>
#include <codac2_AnalyticFunction.h>
#include "codac2_py_AnalyticFunction_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_analytic_operations_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_doc.h"

#include "codac2_py_wrapper.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


template<typename T>
void export_AnalyticFunction(py::module& m, const std::string& export_name)
{
  py::class_<AnalyticFunction<T>, std::shared_ptr<AnalyticFunction<T>> /* due to enable_shared_from_this */>
    exported(m, export_name.c_str(), ANALYTICFUNCTION_MAIN);
  exported
  
    .def(py::init(
      [](const py::list& l, const ExprWrapper<T>& e)
      {
        vector<std::reference_wrapper<VarBase>> args;

        for(const auto& li : l)
        {
          if(py::isinstance<ScalarVar>(li))
            args.push_back(*std::make_shared<ScalarVar>(li.cast<ScalarVar>()));

          else if(py::isinstance<VectorVar>(li))
            args.push_back(*std::make_shared<VectorVar>(li.cast<VectorVar>()));

          else { assert(false && "unhandlded case"); }
        }

        return std::make_unique<AnalyticFunction<T>>(args, e._e);
      }
    ), DOC_TO_BE_DEFINED)

    .def("eval", [](AnalyticFunction<T>& f, const Interval& x) { return f.eval(x); }, 
      DOC_TO_BE_DEFINED)

    .def("__repr__", [](const AnalyticFunction<T>& f) {
          std::ostringstream stream;
          stream << f;
          return string(stream.str()); 
        },
      DOC_TO_BE_DEFINED)
  ;
}

template<typename T>
void export_ExprWrapper(py::module& m, const std::string& export_name)
{
  py::class_<ExprWrapper<T>>
    exported(m, export_name.c_str(), "todo");
  exported
  
    .def(py::init<ScalarVar>(),
      DOC_TO_BE_DEFINED)
  
    .def(py::init<VectorVar>(),
      DOC_TO_BE_DEFINED)
  
    .def("is_scalar", &ExprWrapper<T>::is_scalar)
  ;
}

inline void export_expression_operations(py::module& m)
{
  m

    .def("cos", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(cos(e._e)); },
      SCALAREXPR_PTR_COS_CONST_SCALAREXPR_PTR_REF,
      "x1"_a)

    .def("vec", [](const ExprWrapper<ScalarOpValue>& e1, const ExprWrapper<ScalarOpValue>& e2) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e)); },
      DOC_TO_BE_DEFINED,
      "x1"_a,"x2"_a)
  ;
}