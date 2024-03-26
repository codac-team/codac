/** 
 *  \file
 *  Interval Python binding
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
#include <codac2_Interval.h>
#include "codac2_py_Interval_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<Interval> export_Interval(py::module& m)
{
  m.attr("oo") = oo;

  py::class_<Interval> exported_interval_class(m, "Interval", INTERVAL_MAIN);
  exported_interval_class
  
    .def(py::init<>(),
      INTERVAL_INTERVAL)

    .def(py::init<double>(),
      INTERVAL_INTERVAL_DOUBLE,
      "a"_a)

    .def(py::init<double,double>(),
      INTERVAL_INTERVAL_DOUBLE_DOUBLE,
      "a"_a, "b"_a)
  
    .def(py::init<const Interval&>(),
      INTERVAL_INTERVAL_CONSTINTERVAL)

    // Interval(std::array<double,1> array);
    // Interval(std::array<double,2> array);

    .def("init", &Interval::init,
      INTERVAL_INTERVAL_INIT_CONSTINTERVAL,
      "x"_a)

    .def(py::self == py::self,
      INTERVAL_BOOL_OPERATOREQ_CONSTINTERVAL,
      "x"_a)

    .def(py::self != py::self,
      INTERVAL_BOOL_OPERATORNEQ_CONSTINTERVAL,
      "x"_a)

    .def("lb", &Interval::lb,
      INTERVAL_DOUBLE_LB)

    .def("ub", &Interval::ub,
      INTERVAL_DOUBLE_UB)

    .def("mid", &Interval::mid,
      INTERVAL_DOUBLE_MID)

    .def("rad", &Interval::rad,
      INTERVAL_DOUBLE_RAD)

    .def("diam", &Interval::diam,
      INTERVAL_DOUBLE_DIAM)

    .def("volume", &Interval::volume,
      INTERVAL_DOUBLE_VOLUME)

    .def("size", &Interval::size,
      INTERVAL_SIZET_SIZE)

    .def("set_empty", &Interval::set_empty,
      INTERVAL_VOID_SET_EMPTY)

    .def("is_empty", &Interval::is_empty,
      INTERVAL_BOOL_IS_EMPTY)

    .def("contains", &Interval::contains,
      INTERVAL_BOOL_CONTAINS_CONSTDOUBLE,
      "x"_a)

    .def("__contains__", &Interval::contains,
      INTERVAL_BOOL_CONTAINS_CONSTDOUBLE,
      "x"_a)

    .def("interior_contains", &Interval::interior_contains,
      INTERVAL_BOOL_INTERIOR_CONTAINS_CONSTDOUBLE,
      "x"_a)

    .def("is_unbounded", &Interval::is_unbounded,
      INTERVAL_BOOL_IS_UNBOUNDED)

    .def("is_degenerated", &Interval::is_degenerated,
      INTERVAL_BOOL_IS_DEGENERATED)

    .def("intersects", &Interval::intersects,
      INTERVAL_BOOL_INTERSECTS_CONSTINTERVAL,
      "x"_a)

    .def("is_disjoint", &Interval::is_disjoint,
      INTERVAL_BOOL_IS_DISJOINT_CONSTINTERVAL,
      "x"_a)

    .def("overlaps", &Interval::overlaps,
      INTERVAL_BOOL_OVERLAPS_CONSTINTERVAL,
      "x"_a)

    .def("is_subset", &Interval::is_subset,
      INTERVAL_BOOL_IS_SUBSET_CONSTINTERVAL,
      "x"_a)

    .def("is_strict_subset", &Interval::is_strict_subset,
      INTERVAL_BOOL_IS_STRICT_SUBSET_CONSTINTERVAL,
      "x"_a)

    .def("is_interior_subset", &Interval::is_interior_subset,
      INTERVAL_BOOL_IS_INTERIOR_SUBSET_CONSTINTERVAL,
      "x"_a)

    .def("is_strict_interior_subset", &Interval::is_strict_interior_subset,
      INTERVAL_BOOL_IS_STRICT_INTERIOR_SUBSET_CONSTINTERVAL,
      "x"_a)

    .def("is_superset", &Interval::is_superset,
      INTERVAL_BOOL_IS_SUPERSET_CONSTINTERVAL,
      "x"_a)

    .def("is_strict_superset", &Interval::is_strict_superset,
      INTERVAL_BOOL_IS_STRICT_SUPERSET_CONSTINTERVAL,
      "x"_a)

    .def("inflate", &Interval::inflate,
      INTERVAL_INTERVAL_INFLATE_CONSTDOUBLE,
      "rad"_a)

    .def("is_bisectable", &Interval::is_bisectable,
      INTERVAL_BOOL_IS_BISECTABLE)

    .def("bisect", &Interval::bisect,
      INTERVAL_PAIRINTERVALINTERVAL_BISECT_FLOAT,
      "ratio"_a = 0.49)

    .def("complementary", &Interval::complementary,
      INTERVAL_VECTORINTERVAL_COMPLEMENTARY_BOOL,
      "compactness"_a = true)

    .def("diff", &Interval::diff,
      INTERVAL_VECTORINTERVAL_DIFF_CONSTINTERVAL_BOOL,
      "y"_a, "compactness"_a = true)

    .def(py::self |= py::self,
      INTERVAL_INTERVAL_OPERATORUNIEQ_CONSTINTERVAL,
      "x"_a)

    // For MATLAB compatibility
    .def("self_union", &Interval::operator|=,
      INTERVAL_INTERVAL_OPERATORUNIEQ_CONSTINTERVAL,
      "x"_a)

    .def(py::self &= py::self,
      INTERVAL_INTERVAL_OPERATORINTEQ_CONSTINTERVAL,
      "x"_a)

    // For MATLAB compatibility
    .def("self_inter", &Interval::operator&=,
      INTERVAL_INTERVAL_OPERATORINTEQ_CONSTINTERVAL,
      "x"_a)

    .def(py::self += double(),
      INTERVAL_INTERVAL_OPERATORADDEQ_DOUBLE,
      "x"_a)

    .def(py::self += py::self,
      INTERVAL_INTERVAL_OPERATORADDEQ_CONSTINTERVAL,
      "x"_a)

    .def(-py::self,
      INTERVAL_INTERVAL_OPERATORMIN)

    .def(py::self -= double(),
      INTERVAL_INTERVAL_OPERATORMINEQ_DOUBLE,
      "x"_a)

    .def(py::self -= py::self,
      INTERVAL_INTERVAL_OPERATORMINEQ_CONSTINTERVAL,
      "x"_a)

    .def(py::self *= double(),
      INTERVAL_INTERVAL_OPERATORMULEQ_DOUBLE,
      "x"_a)

    .def(py::self *= py::self,
      INTERVAL_INTERVAL_OPERATORMULEQ_CONSTINTERVAL,
      "x"_a)

    .def(py::self /= double(),
      INTERVAL_INTERVAL_OPERATORDIVEQ_DOUBLE,
      "x"_a)

    .def(py::self /= py::self,
      INTERVAL_INTERVAL_OPERATORDIVEQ_CONSTINTERVAL,
      "x"_a)

    .def_static("empty", &Interval::empty,
      INTERVAL_INTERVAL_EMPTY)

    .def_static("half_pi", &Interval::half_pi,
      INTERVAL_INTERVAL_HALF_PI)

    .def_static("pi", &Interval::pi,
      INTERVAL_INTERVAL_PI)

    .def_static("two_pi", &Interval::two_pi,
      INTERVAL_INTERVAL_TWO_PI)

    .def("__repr__", [](const Interval& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      INTERVAL_OSTREAM_OPERATOR_OSTREAM_CONSTINTERVAL)
  ;

  // Automatic cast lists to Intervals
  py::implicitly_convertible<py::list,Interval>();
  py::implicitly_convertible<double,Interval>();
  py::implicitly_convertible<int,Interval>();

  // The following functions are defined in GAOL:

    m.def("previous_float", &previous_float,
      "Returns the previous floating point value",
      "x"_a);

    m.def("next_float", &next_float,
      "Returns the next floating point value",
      "x"_a);

  return exported_interval_class;
}