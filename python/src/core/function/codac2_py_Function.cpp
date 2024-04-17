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

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Arg.h>
#include <codac2_Function.h>
#include <codac2_Expr_operations.h>
#include "codac2_py_Function_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

#include "codac2_py_wrapper.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


std::unique_ptr<ScalarFunction> create_scalar_function(const py::list& l, const ExprWrapper& e)
{
  vector<std::reference_wrapper<ArgBase>> args;
  Arg x;

  for(const auto& li : l)
  {
    if(py::isinstance<Arg>(li))
      args.push_back(*std::make_shared<Arg>(li.cast<Arg>()));

    else if(py::isinstance<ArgVector>(li))
      args.push_back(*std::make_shared<ArgVector>(li.cast<ArgVector>()));

    else { assert(false && "unhandlded case"); }
  }

  return std::make_unique<ScalarFunction>(args, e._e);
}

void export_ScalarFunction(py::module& m)
{
  py::class_<ScalarFunction, std::shared_ptr<ScalarFunction> /* due to enable_shared_from_this */>
    exported(m, "ScalarFunction", FUNCTION_MAIN);
  exported
  
    .def(py::init(&create_scalar_function),
      DOC_TO_BE_DEFINED)

    .def("eval", [](ScalarFunction& f, const Interval& x) { return f.eval(x); }, 
      DOC_TO_BE_DEFINED)

  ;
}

void export_ExprWrapper(py::module& m)
{
  py::class_<ExprWrapper>
    exported(m, "ExprWrapper", DOC_TO_BE_DEFINED);
  exported
  
    .def(py::init<Arg>(),
      DOC_TO_BE_DEFINED)

  ;
}

void export_expression_operations(py::module& m)
{
  m

    .def("cos", [](const ExprWrapper& e) { return ExprWrapper(cos(e._e)); },
      DOC_TO_BE_DEFINED,
      "x1"_a)
  ;
}