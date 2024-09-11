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
#include <codac2_Polygon.h>
#include "codac2_py_Polygon_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Polygon(py::module& m)
{
  py::class_<Polygon> exported(m, "Polygon", POLYGON_MAIN);
  exported

    .def(py::init<const std::vector<Vector>&>(),
      POLYGON_POLYGON_CONST_VECTOR_VECTOR_REF,
      "vertices"_a)

    .def(py::init<const std::vector<Edge>&>(),
      POLYGON_POLYGON_CONST_VECTOR_EDGE_REF,
      "edges"_a)

    .def("edges", &Polygon::edges,
      CONST_VECTOR_EDGE_REF_POLYGON_EDGES_CONST)

    .def("contains", &Polygon::contains,
      BOOLINTERVAL_POLYGON_CONTAINS_CONST_VECTOR_REF_CONST,
      "p"_a)

  ;

  py::implicitly_convertible<py::list,Polygon>();
}