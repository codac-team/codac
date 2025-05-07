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
#include <codac2_ConvexPolygon.h>
#include "codac2_py_ConvexPolygon_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_ConvexPolygon(py::module& m)
{
  py::class_<ConvexPolygon,Polygon> exported(m, "ConvexPolygon", CONVEXPOLYGON_MAIN);
  exported

    .def(py::init<const std::vector<Vector>&>(),
      CONVEXPOLYGON_CONVEXPOLYGON_CONST_VECTOR_VECTOR_REF,
      "vertices"_a)

    .def(py::init<const std::vector<Segment>&>(),
      CONVEXPOLYGON_CONVEXPOLYGON_CONST_VECTOR_SEGMENT_REF,
      "edges"_a)

    .def(py::init<const IntervalVector&>(),
      CONVEXPOLYGON_CONVEXPOLYGON_CONST_INTERVALVECTOR_REF,
      "x"_a)

    .def("__and__",
        [](const ConvexPolygon& p1, const ConvexPolygon& p2) -> ConvexPolygon
        {
          return p1 & p2;
        },
      CONVEXPOLYGON_OPERATORAND_CONST_CONVEXPOLYGON_REF_CONST_CONVEXPOLYGON_REF,
      "p2"_a)

    .def_static("empty", &ConvexPolygon::empty,
      STATIC_CONVEXPOLYGON_CONVEXPOLYGON_EMPTY)

  ;

  py::implicitly_convertible<py::list,ConvexPolygon>();
}