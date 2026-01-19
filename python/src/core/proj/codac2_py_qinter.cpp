/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <codac2_qinter.h>
#include "codac2_py_qinter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_qinter(py::module& m)
{
  m.def("qinter", &codac2::qinter,
    INTERVALVECTOR_QINTER_UNSIGNED_INT_CONST_LIST_INTERVALVECTOR_REF,
    "q"_a, "l"_a);
}