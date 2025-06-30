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

    .def(py::init<const std::vector<Segment>&>(),
      POLYGON_POLYGON_CONST_VECTOR_SEGMENT_REF,
      "edges"_a)

    .def("edges", &Polygon::edges,
      CONST_VECTOR_SEGMENT_REF_POLYGON_EDGES_CONST)

    .def("unsorted_vertices", &Polygon::unsorted_vertices,
      LIST_INTERVALVECTOR_POLYGON_UNSORTED_VERTICES_CONST)

    .def("sorted_vertices", &Polygon::sorted_vertices,
      VECTOR_INTERVALVECTOR_POLYGON_SORTED_VERTICES_CONST)

    .def("box", &Polygon::box,
      INTERVALVECTOR_POLYGON_BOX_CONST)

    .def("is_empty", &Polygon::is_empty,
      BOOL_POLYGON_IS_EMPTY_CONST)

    .def("contains", &Polygon::contains,
      BOOLINTERVAL_POLYGON_CONTAINS_CONST_INTERVALVECTOR_REF_CONST,
      "p"_a)
    
    .def(py::self == py::self,
      BOOL_POLYGON_OPERATOREQ_CONST_POLYGON_REF_CONST,
      "p"_a)

    .def("__repr__", [](const Polygon& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_POLYGON_REF)

    .def_static("empty", &Polygon::empty,
      STATIC_POLYGON_POLYGON_EMPTY)

  ;

  py::implicitly_convertible<py::list,Polygon>();
}