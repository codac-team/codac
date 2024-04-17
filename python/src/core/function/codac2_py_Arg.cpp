/** 
 *  \file
 *  Arg Python binding
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
#include <codac2_Arg.h>
#include <codac2_Expr.h>
#include "codac2_py_Arg_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

#include "codac2_py_wrapper.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Arg(py::module& m)
{
  py::class_<Arg, std::shared_ptr<Arg> /* due to enable_shared_from_this */>
    exported(m, "Arg", DOC_TO_BE_DEFINED);
  exported
  
    .def(py::init<>(),
      DOC_TO_BE_DEFINED)

  ;

  py::implicitly_convertible<Arg,ExprWrapper>();
}

void export_ArgVector(py::module& m)
{
  py::class_<ArgVector,
    std::shared_ptr<ArgVector> /* due to enable_shared_from_this */>
    exported(m, "ArgVector", DOC_TO_BE_DEFINED);
  exported
  
    .def(py::init<size_t>(),
      DOC_TO_BE_DEFINED)

  ;
}