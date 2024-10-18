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
#include <codac2_Edge.h>
#include "codac2_py_Edge_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Edge(py::module& m)
{
  py::class_<Edge> exported(m, "Edge", EDGE_MAIN);
  exported

    .def(py::init<const std::vector<Vector>&>(),
      EDGE_EDGE_CONST_VECTOR_VECTOR_REF,
      "vertices"_a)

    .def("intersects", &Edge::intersects,
      BOOLINTERVAL_EDGE_INTERSECTS_CONST_EDGE_REF_CONST,
      "e"_a)
  ;

  py::implicitly_convertible<py::list,Edge>();
}