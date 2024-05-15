/** 
 *  \file
 *  ScalarVar Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
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
#include "codac2_py_doc.h"

#include "codac2_py_wrapper.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Var(py::module& m)
{
  py::class_<ScalarVar, std::shared_ptr<ScalarVar> /* due to enable_shared_from_this */>
    exported(m, "ScalarVar", DOC_TO_BE_DEFINED);
  exported
  
    .def(py::init<>(),
      DOC_TO_BE_DEFINED)

  ;

  py::implicitly_convertible<ScalarVar,ExprWrapper>();
}

void export_VectorVar(py::module& m)
{
  py::class_<VectorVar,
    std::shared_ptr<VectorVar> /* due to enable_shared_from_this */>
    exported(m, "VectorVar", DOC_TO_BE_DEFINED);
  exported
  
    .def(py::init<size_t>(),
      DOC_TO_BE_DEFINED)

  ;
}