/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_geometry.h>
#include "codac2_py_geometry_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_geometry(py::module& m)
{
  m.def("orientation", &orientation,
    BOOLINTERVAL_ORIENTATION_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
    "p1"_a, "p2"_a, "p3"_a);
}