/** 
 *  \file
 *  Interval Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <string>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include "codac_type_caster.h"

#include "codac_Interval.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_Interval_docs.h" // todo: generate this file from Doxygen doc

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;

double get_item(Interval& x, size_t i)
{
  if(i == 0)
    return x.lb();
  else if(i == 1)
    return x.ub();
  else
    throw py::index_error();
}

Interval interval_copy(Interval& itv)
{
  return Interval(itv);
}

unique_ptr<Interval> build_from_list(vector<double>& lst)
{
  if(lst.size() != 2)
    throw invalid_argument("Argument number != 2");
  return unique_ptr<Interval>(new Interval(lst[0], lst[1]));
}

void assign_interval(Interval& x, const Interval& y)
{
  x = y;
}

string to_string(const Interval& a)
{
  stringstream ss;
  ss << a;
  return ss.str();
}

py::tuple diff_wrapper(const Interval& x, const Interval& y)
{
  Interval c1, c2;
  x.diff(y, c1, c2);
  return py::make_tuple(c1, c2);
}

py::tuple complementary_wrapper(const Interval& x)
{
  Interval c1, c2;
  x.complementary(c1, c2);
  return py::make_tuple(c1, c2);
}

void export_Interval(py::module& m)
{
  py::class_<Interval> interval(m, "Interval", DOCS_INTERVAL_TYPE);
  interval

  // Definition

    .def(py::init<>())
    .def(py::init<Interval>(), "build a copy of an interval", "x"_a.noconvert())
    .def(py::init<double, double>(), "build the interval [lb,ub]", "lb"_a, "ub"_a)
    .def(py::init<double>(), "build singleton [x,x]", "x"_a)
    //.def(py::init(&build_from_list))
    //.def(py::init([](array<double, 2>& bounds) {return new Interval(bounds[0], bounds[1]);}))
    //.def(py::init([](array<int, 2>& bounds)    {return new Interval(bounds[0], bounds[1]);}))
    //.def(py::init([](pair<int, double>& bounds) {return new Interval(bounds.first, bounds.second);}))
    //.def(py::init([](pair<double, int>& bounds) {return new Interval(bounds.first, bounds.second);}))
    //.def("__init__", [](Interval &instance,  pair<int, double>& bounds) { new(&instance) Interval(double(getw<0>(bounds)), get<1>(bounds[1]));})
    //.def("__init__", [](Interval &instance,  pair<int, double>& bounds) { new(&instance) Interval(double(bounds.first), bounds.second);})

    .def("assign", &assign_interval, "assign the value of x to this", "x"_a)
    .def("__repr__", [](const Interval& x) { ostringstream str; str << x; return str.str(); })

    .def("__getitem__", [](Interval& s, size_t index) -> double
      {
        if(index < 0 || index > 1)
          throw py::index_error();

        cout << "Warning: indexing on intervals is deprecated." << endl
             << "         Use .lb(), .ub() methods instead of []." << endl;

        if(index == 0) return s.lb();
        else if(index == 1) return s.ub();
        else return NAN;
      },
      py::return_value_policy::reference_internal)
  
  // Arithmetic

    .def(py::self == py::self)
    .def(py::self + py::self)
    .def(py::self - py::self)
    .def(py::self * py::self)
    .def(py::self / py::self)
    .def(py::self & py::self)
    .def(py::self | py::self)

    .def("__iadd__", [](Interval& x, Interval& o) { return x += o; })
    .def("__isub__", [](Interval& x, Interval& o) { return x -= o; })
    .def("__isub__", [](Interval& x, double& o) { return x -= o; })
    .def("__imul__", [](Interval& x, Interval& o) { return x *= o; })
    .def("__ifloordiv__", [](Interval& x, Interval& o) { return x /= o; })

    .def(py::self &= py::self)
    .def(py::self |= py::self)

    .def(py::self + double())
    .def(py::self += double())
    .def(double() + py::self)
    .def(py::self - double())
    .def(double() - py::self)
    .def(py::self * double())
    .def(double() * py::self)
    .def(py::self / double())
    .def(double() / py::self)

    //.def( "__mul__", [](Interval& x, IntervalVector& a) { return x*a; })

    .def(-py::self)

    .def("__abs__", [](const Interval& a) { return ibex::abs(a); })
    .def("__pow__", [](const Interval& x, int n) { return ibex::pow(x, n); })
    .def("__pow__", [](const Interval& x, double d) { return ibex::pow(x, d); })
    .def("__pow__", [](const Interval &x, const Interval &y) { return ibex::pow(x, y); })

    .def("lb", &Interval::lb, "return the upper bound")
    .def("ub", &Interval::ub, "return the lower bound")
    .def("inflate", (Interval& (Interval::*) (double)) &Interval::inflate, DOCS_INTERVAL_INFLATE)
    .def("inflate", (Interval& (Interval::*) (double, double)) &Interval::inflate, DOCS_INTERVAL_INFLATE)
    //.def("inflate", &Interval::inflate, DOCS_INTERVAL_INFLATE, py::return_value_policy::copy)
    .def("set_empty", &Interval::set_empty, "set x to empty set")
    .def("mid", &Interval::mid, DOCS_INTERVAL_MID)
    .def("rad", &Interval::rad, DOCS_INTERVAL_RAD)
    .def("diam", &Interval::diam, DOCS_INTERVAL_DIAM)
    .def("mig", &Interval::mig, DOCS_INTERVAL_MIG)
    .def("mag", &Interval::mag, DOCS_INTERVAL_MAG)

  // Tests

    .def("is_subset", &Interval::is_subset, DOCS_INTERVAL_IS_SUBSET, "x"_a)
    .def("is_strict_subset", &Interval::is_strict_subset, DOCS_INTERVAL_IS_STRICT_SUBSET, "x"_a)
    .def("is_interior_subset", &Interval::is_interior_subset, DOCS_INTERVAL_IS_INTERIOR_SUBSET, "x"_a)
    .def("is_strict_interior_subset", &Interval::is_strict_interior_subset, DOCS_INTERVAL_IS_STRICT_INTERIOR_SUBSET, "x"_a)
    .def("is_superset", &Interval::is_superset, DOCS_INTERVAL_IS_SUPERSET, "x"_a)
    .def("is_strict_superset", &Interval::is_strict_superset, DOCS_INTERVAL_IS_STRICT_SUPERSET, "x"_a)
    .def("contains", &Interval::contains, DOCS_INTERVAL_CONTAINS, "x"_a)
    .def("__contains__", &Interval::contains, DOCS_INTERVAL_CONTAINS, "x"_a)
    .def("interior_contains", &Interval::interior_contains, DOCS_INTERVAL_INTERIOR_CONTAINS, "x"_a)
    .def("intersects", &Interval::intersects, DOCS_INTERVAL_INTERSECTS, "x"_a)
    .def("overlaps", &Interval::overlaps, DOCS_INTERVAL_OVERLAPS, "x"_a)
    .def("is_disjoint", &Interval::is_disjoint, DOCS_INTERVAL_IS_DISJOINT, "x"_a)
    .def("is_empty", &Interval::is_empty, DOCS_INTERVAL_IS_EMPTY)
    .def("is_degenerated", &Interval::is_degenerated, DOCS_INTERVAL_IS_DEGENERATED)
    .def("is_unbounded", &Interval::is_unbounded, DOCS_INTERVAL_IS_UNBOUNDED)
    .def("is_bisectable", &Interval::is_bisectable,DOCS_INTERVAL_IS_BISECTABLE)
    .def("rel_distance", &Interval::rel_distance, DOCS_INTERVAL_REL_DISTANCE, "x"_a)
    .def("complementary", &complementary_wrapper, DOCS_INTERVAL_COMPLEMENTARY)
    .def("diff", &diff_wrapper, DOCS_INTERVAL_DIFF, "y"_a)

    .def(py::pickle(
      [](const Interval &p) {
        return py::make_tuple(p.lb(), p.ub());
      },
      [](py::tuple t) {
        if(t.size() != 2)
          throw runtime_error("Invalid state");
        auto p = Interval(t[0].cast<double>(), t[1].cast<double>());
        return p;
      }
    ))

    .def("bisect", &Interval::bisect, DOCS_INTERVAL_BISECT, py::arg("ratio")=0.5)
    .def("__get_item__", get_item, "x[0] returns the lb and x[1] returns ub")
    .def("copy", &interval_copy, "return a new object which is the copy of x")
    .def("__hash__", [](const Interval& s1) { return reinterpret_cast<std::uintptr_t>(&s1); })
    //.def( "__pow__", pow__)

  // Constants

    .def_property_readonly_static("PI", [](py::object) { return Interval::PI; })
    .def_property_readonly_static("TWO_PI", [](py::object) { return Interval::TWO_PI; })
    .def_property_readonly_static("HALF_PI", [](py::object) { return Interval::HALF_PI; })
    .def_property_readonly_static("EMPTY_SET", [](py::object) { return Interval::EMPTY_SET; })
    .def_property_readonly_static("ALL_REALS", [](py::object) { return Interval::ALL_REALS; })
    .def_property_readonly_static("ZERO", [](py::object) { return Interval::ZERO; })
    .def_property_readonly_static("ONE", [](py::object) { return Interval::ONE; })
    .def_property_readonly_static("POS_REALS", [](py::object) { return Interval::POS_REALS; })
    .def_property_readonly_static("NEG_REALS", [](py::object) { return Interval::NEG_REALS; })
    //.def_property_readonly_static("ALL_REALS", [](py::object){ return Interval::ALL_REALS; })
  ;

  // External forward functions

  m.def("sqr" ,    (Interval (*) (const Interval&)) &ibex::sqr);
  m.def("sqrt",    (Interval (*) (const Interval&)) &ibex::sqrt);
  m.def("root",    (Interval (*) (const Interval&, int)) &ibex::root);
  m.def("exp" ,    (Interval (*) (const Interval&)) &ibex::exp);
  m.def("log" ,    (Interval (*) (const Interval&)) &ibex::log);
  m.def("cos" ,    (Interval (*) (const Interval&)) &ibex::cos);
  m.def("sin" ,    (Interval (*) (const Interval&)) &ibex::sin);
  m.def("tan" ,    (Interval (*) (const Interval&)) &ibex::tan);
  m.def("acos",    (Interval (*) (const Interval&)) &ibex::acos);
  m.def("asin",    (Interval (*) (const Interval&)) &ibex::asin);
  m.def("atan",    (Interval (*) (const Interval&)) &ibex::atan);
  m.def("cosh",    (Interval (*) (const Interval&)) &ibex::cosh);
  m.def("sinh",    (Interval (*) (const Interval&)) &ibex::sinh);
  m.def("tanh",    (Interval (*) (const Interval&)) &ibex::tanh);
  m.def("acosh",   (Interval (*) (const Interval&)) &ibex::acosh);
  m.def("asinh",   (Interval (*) (const Interval&)) &ibex::asinh);
  m.def("atanh",   (Interval (*) (const Interval&)) &ibex::atanh);
  m.def("abs",     (Interval (*) (const Interval&)) &ibex::abs);
  m.def("sign",    (Interval (*) (const Interval&)) &ibex::sign);
  m.def("integer", (Interval (*) (const Interval&)) &ibex::integer);
  m.def("atan2",   (Interval (*) (const Interval&, const Interval&)) &ibex::atan2);
  m.def("max",     (Interval (*) (const Interval&, const Interval&)) &ibex::max);
  m.def("min",     (Interval (*) (const Interval&, const Interval&)) &ibex::min);
  m.def("chi",     (Interval (*) (const Interval&, const Interval&, const Interval&)) &ibex::chi);
  m.def("ceil" ,   (Interval (*) (const Interval&)) &ibex::ceil);
  m.def("floor",   (Interval (*) (const Interval&)) &ibex::floor);

  // Beware: in Python, the arg is given in double by default, not in int
  // Possible bug in pow_2
  // Interval (*pow_1)(const Interval& x, int n) = &ibex::pow;
  // Interval (*pow_2)(const Interval& x, double d) = &ibex::pow;
  // Interval (*pow_3)(const Interval &x, const Interval &y) = &ibex::pow;
  // def("ipow", pow_1);
  // def("ipow", pow_2);
  // def("ipow", pow_3);

  // External backward functions

  m.def("bwd_add",   (bool (*)(const Interval&, Interval&, Interval&)) &ibex::bwd_add);
  m.def("bwd_sub",   (bool (*)(const Interval&, Interval&, Interval&)) &ibex::bwd_sub);
  m.def("bwd_mul",   (bool (*)(const Interval&, Interval&, Interval&)) &ibex::bwd_mul);
  m.def("bwd_div",   &ibex::bwd_div);
  m.def("bwd_sqr",   &ibex::bwd_sqr);
  m.def("bwd_sqrt",  &ibex::bwd_sqrt);
  m.def("bwd_pow",   (bool (*)(const Interval&, Interval&, Interval&)) &ibex::bwd_pow);
  m.def("bwd_pow",   (bool (*)(const Interval&, int, Interval&)) &ibex::bwd_pow);
  m.def("bwd_root",  &ibex::bwd_root);
  m.def("bwd_exp",   &ibex::bwd_exp);
  m.def("bwd_log",   &ibex::bwd_log);
  m.def("bwd_cos",   &ibex::bwd_cos);
  m.def("bwd_sin",   &ibex::bwd_sin);
  m.def("bwd_tan",   &ibex::bwd_tan);
  m.def("bwd_acos",  &ibex::bwd_acos);
  m.def("bwd_asin",  &ibex::bwd_asin);
  m.def("bwd_atan",  &ibex::bwd_atan);
  m.def("bwd_atan2", &ibex::bwd_atan2);
  m.def("bwd_cosh",  &ibex::bwd_cosh);
  m.def("bwd_sinh",  &ibex::bwd_sinh);
  m.def("bwd_tanh",  &ibex::bwd_tanh);
  m.def("bwd_acosh", &ibex::bwd_acosh);
  m.def("bwd_asinh", &ibex::bwd_asinh);
  m.def("bwd_atanh", &ibex::bwd_atanh);
  m.def("bwd_abs",   &ibex::bwd_abs);
  m.def("bwd_max",   &ibex::bwd_max);
  m.def("bwd_min",   &ibex::bwd_min);
  m.def("bwd_sign",  &ibex::bwd_sign);
  m.def("bwd_chi",   &ibex::bwd_chi);
  m.def("bwd_floor", &ibex::bwd_floor);
  m.def("bwd_ceil",  &ibex::bwd_ceil);
  m.def("bwd_imod",  &ibex::bwd_imod);

  // sbool (*bwd_pow_1)(const Interval&, Interval& , Interval&) = &ibex::bwd_pow;
  // sbool (*bwd_pow_2)(const Interval&, int , Interval&) = &ibex::bwd_pow;

  m.attr("oo") = POS_INFINITY;
};