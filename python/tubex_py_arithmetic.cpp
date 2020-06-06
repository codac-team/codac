/** 
 *  \file
 *  Arithmetic binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

#include "tubex_tube_arithmetic.h"
#include "tubex_traj_arithmetic.h"
#include "ibex_Interval.h"

using namespace ibex;
using namespace tubex;

using namespace pybind11::literals;
namespace py = pybind11;
using py::class_;
using py::init;


#define macro_scal_unary(str_f, f) \
  \
  m.def(str_f, (double (*) (double)) &std::f); \
  m.def(str_f, (Interval (*) (const Interval&)) &ibex::f); \
  m.def(str_f, (const Tube (*) (const Tube&)) &f); \
  m.def(str_f, (const Trajectory (*) (const Trajectory&)) &f); \

void export_arithmetic(py::module& m)
{
  macro_scal_unary("cos", cos);
  macro_scal_unary("sin", sin);
  macro_scal_unary("abs", abs);
  macro_scal_unary("sqrt", sqrt);
  macro_scal_unary("exp", exp);
  macro_scal_unary("log", log);
  macro_scal_unary("tan", tan);
  macro_scal_unary("acos", acos);
  macro_scal_unary("asin", asin);
  macro_scal_unary("atan", atan);
  macro_scal_unary("cosh", cosh);
  macro_scal_unary("sinh", sinh);
  macro_scal_unary("tanh", tanh);
  macro_scal_unary("acosh", acosh);
  macro_scal_unary("asinh", asinh);
  macro_scal_unary("atanh", atanh);

  // sqr (not defined in std)
  m.def("sqr", [](double x) { return pow(x,2); }, "x"_a.noconvert());
  m.def("sqr", (Interval (*) (const Interval&)) &ibex::sqr);
  m.def("sqr", (const Tube (*) (const Tube&)) &sqr);
  m.def("sqr", (const Trajectory (*) (const Trajectory&)) &sqr);

  // pow (several possible argument types)
  m.def("pow", (double (*) (double x, int p)) &std::pow, "x"_a, "p"_a);
  m.def("pow", (double (*) (double x, double p)) &std::pow, "x"_a, "p"_a);
  m.def("pow", (Interval (*) (const Interval& x, int p)) &ibex::pow, "x"_a, "p"_a);
  m.def("pow", (Interval (*) (const Interval& x, double p)) &ibex::pow, "x"_a, "p"_a);
  m.def("pow", (Interval (*) (const Interval& x, const Interval& p)) &ibex::pow, "x"_a, "p"_a);
  m.def("pow", [](double x, const Interval& p) { return ibex::pow(Interval(x),p); }, "x"_a, "p"_a);
  m.def("pow", (const Tube (*) (const Tube& x, int p)) &pow, "x"_a, "p"_a);
  m.def("pow", (const Tube (*) (const Tube& x, double p)) &pow, "x"_a, "p"_a);
  m.def("pow", (const Tube (*) (const Tube& x, const Interval& p)) &pow, "x"_a, "p"_a);
  m.def("pow", (const Trajectory (*) (const Trajectory& x, int p)) &pow, "x"_a, "p"_a);
  m.def("pow", (const Trajectory (*) (const Trajectory& x, double p)) &pow, "x"_a, "p"_a);

  // root
  m.def("root", (Interval (*) (const Interval& x, int p)) &ibex::root, "x"_a, "p"_a);
  m.def("root", (const Tube (*) (const Tube& x, int p)) &root, "x"_a, "p"_a);
  m.def("root", (const Trajectory (*) (const Trajectory& x, int p)) &root, "x"_a, "p"_a);

  // atan2
  m.def("atan2", [](double y, double x) { return std::atan2(y,x); }, "y"_a.noconvert(), "x"_a.noconvert());
  m.def("atan2", [](const Interval& y, double x) { return ibex::atan2(y,Interval(x)); }, "y"_a.noconvert(), "x"_a.noconvert());
  m.def("atan2", [](double y, const Interval& x) { return ibex::atan2(Interval(y),x); }, "y"_a.noconvert(), "x"_a.noconvert());
  m.def("atan2", (Interval (*) (const Interval& y, const Interval& x)) &atan2, "y"_a, "x"_a);
  m.def("atan2", (const Tube (*) (const Tube& y, const Tube& x)) &atan2, "y"_a, "x"_a);
  m.def("atan2", [](const Tube& y, double x) { return atan2(y,Interval(x)); } , "y"_a.noconvert(), "x"_a.noconvert());
  m.def("atan2", (const Tube (*) (const Tube& y, const Interval& x)) &atan2, "y"_a, "x"_a);
  m.def("atan2", [](double y, const Tube& x) { return atan2(Interval(y),x); } , "y"_a.noconvert(), "x"_a.noconvert());
  m.def("atan2", (const Tube (*) (const Interval& y, const Tube& x)) &atan2, "y"_a, "x"_a);
  m.def("atan2", (const Trajectory (*) (const Trajectory& y, const Trajectory& x)) &atan2, "y"_a, "x"_a);
  m.def("atan2", (const Trajectory (*) (const Trajectory& y, double x)) &atan2, "y"_a, "x"_a);
  m.def("atan2", (const Trajectory (*) (double y, const Trajectory& x)) &atan2, "y"_a, "x"_a);

  // todo: atan2, pow with Trajectory as parameter
}