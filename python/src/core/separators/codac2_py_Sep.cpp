/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include "codac2_py_Sep.h"
#include "codac2_py_Sep_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<Sep,pySep> export_Sep(py::module& m)
{
  py::class_<Sep,pySep> py_sep(m, "Sep");
  py_sep

    .def(py::init<size_t>(),
      SEP_SEP_SIZET);

  return py_sep;
}