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
#include <codac2_Segment.h>
#include "codac2_py_Segment_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_matlab.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Segment(py::module& m)
{
  py::class_<Segment> exported(m, "Segment", SEGMENT_MAIN);
  exported

    .def(py::init<const std::array<IntervalVector,2>&>(),
      SEGMENT_SEGMENT_CONST_ARRAY_INTERVALVECTOR2_REF,
      "x"_a)

    .def(py::init<const IntervalVector&,const IntervalVector&>(),
      SEGMENT_SEGMENT_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
      "x1"_a, "x2"_a)

    .def("__getitem__", [](const Segment& e, Index_type index) -> const IntervalVector&
        {
          matlab::test_integer(index);
          return e[matlab::input_index(index)];
        }, py::return_value_policy::reference_internal)

    .def("__setitem__", [](Segment& e, Index_type index, const IntervalVector& x)
        {
          matlab::test_integer(index);
          e[matlab::input_index(index)] = x;
        })

    .def("box", &Segment::box,
      INTERVALVECTOR_SEGMENT_BOX_CONST)

    .def("intersects", &Segment::intersects,
      BOOLINTERVAL_SEGMENT_INTERSECTS_CONST_SEGMENT_REF_CONST,
      "e"_a)

    .def("contains", &Segment::contains,
      BOOLINTERVAL_SEGMENT_CONTAINS_CONST_INTERVALVECTOR_REF_CONST,
      "p"_a)

    .def(py::self == py::self,
      BOOL_SEGMENT_OPERATOREQ_CONST_SEGMENT_REF_CONST,
      "e"_a)

    .def("__and__",
        [](const Segment& e1, const Segment& e2) -> IntervalVector
        {
          return e1 & e2;
        },
      INTERVALVECTOR_OPERATORAND_CONST_SEGMENT_REF_CONST_SEGMENT_REF,
      "e2"_a)

    .def("__repr__", [](const Segment& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_SEGMENT_REF)

  ;

  m.def("proj_intersection", &proj_intersection,
    INTERVALVECTOR_PROJ_INTERSECTION_CONST_SEGMENT_REF_CONST_SEGMENT_REF,
    "e1"_a, "e2"_a);

  m.def("colinear", &colinear,
    BOOLINTERVAL_COLINEAR_CONST_SEGMENT_REF_CONST_SEGMENT_REF,
    "e1"_a, "e2"_a);
  
  py::implicitly_convertible<py::list,Segment>();
}