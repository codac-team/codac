/** 
 *  Interval Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Interval.h>
#include "codac2_py_Interval_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_Interval_impl_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<Interval> export_Interval(py::module& m)
{
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
      INTERVAL_INTERVAL_CONST_INTERVAL_REF)

    .def(py::init(
        [](const std::list<double>& l)
        {
          auto i = std::make_unique<Interval>();
          i->init_from_list(l);
          return i;
        }),
      INTERVAL_INTERVAL_INITIALIZER_LIST_DOUBLE,
      "l"_a)

    // Interval(std::array<double,1> array);
    // Interval(std::array<double,2> array);

    .def("init", (Interval& (Interval::*)())&Interval::init,
      INTERVAL_REF_INTERVAL_INIT)

    .def("init", (Interval& (Interval::*)(const Interval&))&Interval::init,
      INTERVAL_REF_INTERVAL_INIT_CONST_INTERVAL_REF,
      "x"_a)

    .def("init_from_list", &Interval::init_from_list,
      INTERVAL_REF_INTERVAL_INIT_FROM_LIST_CONST_LIST_DOUBLE_REF,
      "l"_a)

    .def(py::self == py::self,
      BOOL_INTERVAL_OPERATOREQ_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def(py::self != py::self,
      BOOL_INTERVAL_OPERATORNEQ_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("lb", &Interval::lb,
      DOUBLE_INTERVAL_LB_CONST)

    .def("ub", &Interval::ub,
      DOUBLE_INTERVAL_UB_CONST)

    .def("mid", &Interval::mid,
      DOUBLE_INTERVAL_MID_CONST)

    .def("mag", &Interval::mag,
      DOUBLE_INTERVAL_MAG_CONST)

    .def("mig", &Interval::mig,
      DOUBLE_INTERVAL_MIG_CONST)

    .def("rand", &Interval::rand,
      DOUBLE_INTERVAL_RAND_CONST)

    .def("rad", &Interval::rad,
      DOUBLE_INTERVAL_RAD_CONST)

    .def("diam", &Interval::diam,
      DOUBLE_INTERVAL_DIAM_CONST)

    .def("volume", &Interval::volume,
      DOUBLE_INTERVAL_VOLUME_CONST)

    .def("size", &Interval::size,
      INDEX_INTERVAL_SIZE_CONST)

    .def("set_empty", &Interval::set_empty,
      VOID_INTERVAL_SET_EMPTY)

    .def("is_empty", &Interval::is_empty,
      BOOL_INTERVAL_IS_EMPTY_CONST)

    .def("contains", &Interval::contains,
      BOOL_INTERVAL_CONTAINS_CONST_DOUBLE_REF_CONST,
      "x"_a)

    .def("__contains__", &Interval::contains,
      BOOL_INTERVAL_CONTAINS_CONST_DOUBLE_REF_CONST,
      "x"_a)

    .def("interior_contains", &Interval::interior_contains,
      BOOL_INTERVAL_INTERIOR_CONTAINS_CONST_DOUBLE_REF_CONST,
      "x"_a)

    .def("is_unbounded", &Interval::is_unbounded,
      BOOL_INTERVAL_IS_UNBOUNDED_CONST)

    .def("is_degenerated", &Interval::is_degenerated,
      BOOL_INTERVAL_IS_DEGENERATED_CONST)

    .def("is_integer", &Interval::is_integer,
      BOOL_INTERVAL_IS_INTEGER_CONST)

    .def("has_integer_bounds", &Interval::has_integer_bounds,
      BOOL_INTERVAL_HAS_INTEGER_BOUNDS_CONST)

    .def("intersects", &Interval::intersects,
      BOOL_INTERVAL_INTERSECTS_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("is_disjoint", &Interval::is_disjoint,
      BOOL_INTERVAL_IS_DISJOINT_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("overlaps", &Interval::overlaps,
      BOOL_INTERVAL_OVERLAPS_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("is_subset", &Interval::is_subset,
      BOOL_INTERVAL_IS_SUBSET_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("is_strict_subset", &Interval::is_strict_subset,
      BOOL_INTERVAL_IS_STRICT_SUBSET_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("is_interior_subset", &Interval::is_interior_subset,
      BOOL_INTERVAL_IS_INTERIOR_SUBSET_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("is_strict_interior_subset", &Interval::is_strict_interior_subset,
      BOOL_INTERVAL_IS_STRICT_INTERIOR_SUBSET_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("is_superset", &Interval::is_superset,
      BOOL_INTERVAL_IS_SUPERSET_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("is_strict_superset", &Interval::is_strict_superset,
      BOOL_INTERVAL_IS_STRICT_SUPERSET_CONST_INTERVAL_REF_CONST,
      "x"_a)

    .def("inflate", &Interval::inflate,
      INTERVAL_REF_INTERVAL_INFLATE_CONST_DOUBLE_REF,
      "rad"_a)

    .def("is_bisectable", &Interval::is_bisectable,
      BOOL_INTERVAL_IS_BISECTABLE_CONST)

    .def("bisect", &Interval::bisect,
      PAIR_INTERVALINTERVAL_INTERVAL_BISECT_FLOAT_CONST,
      "ratio"_a = 0.49)

    .def("complementary", &Interval::complementary,
      VECTOR_INTERVAL_INTERVAL_COMPLEMENTARY_BOOL_CONST,
      "compactness"_a = true)

    .def("diff", &Interval::diff,
      VECTOR_INTERVAL_INTERVAL_DIFF_CONST_INTERVAL_REF_BOOL_CONST,
      "y"_a, "compactness"_a = true)

    .def(py::self |= py::self,
      INTERVAL_REF_INTERVAL_OPERATOROREQ_CONST_INTERVAL_REF,
      "x"_a)

    // For MATLAB compatibility
    .def("self_union", &Interval::operator|=,
      INTERVAL_REF_INTERVAL_OPERATOROREQ_CONST_INTERVAL_REF,
      "x"_a)

    .def(py::self &= py::self,
      INTERVAL_REF_INTERVAL_OPERATORANDEQ_CONST_INTERVAL_REF,
      "x"_a)

    // For MATLAB compatibility
    .def("self_inter", &Interval::operator&=,
      INTERVAL_REF_INTERVAL_OPERATORANDEQ_CONST_INTERVAL_REF,
      "x"_a)

    .def(py::self += double(),
      INTERVAL_REF_INTERVAL_OPERATORPLUSEQ_DOUBLE,
      "x"_a)

    .def(py::self += py::self,
      INTERVAL_REF_INTERVAL_OPERATORPLUSEQ_CONST_INTERVAL_REF,
      "x"_a)

    .def(-py::self,
      INTERVAL_INTERVAL_OPERATORMINUS_CONST)

    .def(py::self -= double(),
      INTERVAL_REF_INTERVAL_OPERATORMINUSEQ_DOUBLE,
      "x"_a)

    .def(py::self -= py::self,
      INTERVAL_REF_INTERVAL_OPERATORMINUSEQ_CONST_INTERVAL_REF,
      "x"_a)

    .def(py::self *= double(),
      INTERVAL_REF_INTERVAL_OPERATORMULEQ_DOUBLE,
      "x"_a)

    .def(py::self *= py::self,
      INTERVAL_REF_INTERVAL_OPERATORMULEQ_CONST_INTERVAL_REF,
      "x"_a)

    .def(py::self /= double(),
      INTERVAL_REF_INTERVAL_OPERATORDIVEQ_DOUBLE,
      "x"_a)

    .def(py::self /= py::self,
      INTERVAL_REF_INTERVAL_OPERATORDIVEQ_CONST_INTERVAL_REF,
      "x"_a)

    .def_static("empty", &Interval::empty,
      STATIC_INTERVAL_INTERVAL_EMPTY)

    .def_static("zero", &Interval::zero,
      STATIC_INTERVAL_INTERVAL_ZERO)

    .def_static("one", &Interval::one,
      STATIC_INTERVAL_INTERVAL_ONE)

    .def_static("half_pi", &Interval::half_pi,
      STATIC_INTERVAL_INTERVAL_HALF_PI)

    .def_static("pi", &Interval::pi,
      STATIC_INTERVAL_INTERVAL_PI)

    .def_static("two_pi", &Interval::two_pi,
      STATIC_INTERVAL_INTERVAL_TWO_PI)

    .def("__repr__", [](const Interval& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_INTERVAL_REF)
  ;

  // Automatic cast lists to Intervals
  py::implicitly_convertible<py::list,Interval>();
  py::implicitly_convertible<double,Interval>();
  py::implicitly_convertible<int,Interval>();

  // The following functions are defined in GAOL:

    m.def("previous_float", &codac2::previous_float,
      "Returns the previous floating point value",
      "x"_a);

    m.def("next_float", &codac2::next_float,
      "Returns the next floating point value",
      "x"_a);

  return exported_interval_class;
}
