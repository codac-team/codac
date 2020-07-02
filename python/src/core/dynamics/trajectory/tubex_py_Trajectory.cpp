/** 
 *  \file
 *  Trajectory Python binding
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
#include "pyIbex_type_caster.h"

#include "tubex_Trajectory.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_Trajectory_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Trajectory(py::module& m)
{
  py::class_<Trajectory> trajectory(m, "Trajectory", TRAJECTORY_MAIN);
  trajectory

  // Definition

    .def(py::init<>(),
      TRAJECTORY_TRAJECTORY)

    .def(py::init<const ibex::Interval&,const TFunction&>(),
      TRAJECTORY_TRAJECTORY_INTERVAL_TFUNCTION,
      "domain"_a, "f"_a)

    .def(py::init<const ibex::Interval&,const TFunction&,double>(),
      TRAJECTORY_TRAJECTORY_INTERVAL_TFUNCTION_DOUBLE,
      "domain"_a, "f"_a, "timestep"_a)

    .def(py::init<const std::map<double,double> &>(),
      TRAJECTORY_TRAJECTORY_MAPDOUBLEDOUBLE,
      "m_map_values"_a)

    .def(py::init<const Trajectory&>(),
      TRAJECTORY_TRAJECTORY_TRAJECTORY,
      "traj"_a)

    .def("size", &Trajectory::size,
      TRAJECTORY_INT_SIZE)

    .def("tdomain", &Trajectory::tdomain,
      TRAJECTORY_CONSTINTERVAL_TDOMAIN)

    .def("definition_type", &Trajectory::definition_type,
      TRAJECTORY_TRAJDEFNTYPE_DEFINITION_TYPE)
    // todo: export TrajDefnType

  // Accessing values

    .def("sampled_map", &Trajectory::sampled_map,
      TRAJECTORY_CONSTMAPDOUBLEDOUBLE_SAMPLED_MAP)

    .def("tfunction", &Trajectory::tfunction,
      TRAJECTORY_CONSTTFUNCTION_TFUNCTION,
      py::return_value_policy::reference_internal)

    .def("codomain", &Trajectory::codomain,
      TRAJECTORY_CONSTINTERVAL_CODOMAIN)

    .def("__call__", [](Trajectory& s,double o) { return s(o); }, 
      TRAJECTORY_DOUBLE_OPERATORP_DOUBLE)

    .def("__call__", [](Trajectory& s,const ibex::Interval& o) { return s(o); }, 
      TRAJECTORY_CONSTINTERVAL_OPERATORP_INTERVAL)

    .def("first_value", &Trajectory::first_value,
      TRAJECTORY_DOUBLE_FIRST_VALUE)

    .def("last_value", &Trajectory::last_value,
      TRAJECTORY_DOUBLE_LAST_VALUE)

  // Tests

    .def("not_defined", &Trajectory::not_defined,
      TRAJECTORY_BOOL_NOT_DEFINED)

    .def("__eq__", [](Trajectory& s,const Trajectory& o) { return s == o; }, 
      TRAJECTORY_BOOL_OPERATOREQ_TRAJECTORY)

    .def("__ne__", [](Trajectory& s,const Trajectory& o) { return s != o; }, 
      TRAJECTORY_BOOL_OPERATORNEQ_TRAJECTORY)

  // Setting values

    .def("set", &Trajectory::set,
      TRAJECTORY_VOID_SET_DOUBLE_DOUBLE,
      "y"_a, "t"_a)

    .def("truncate_tdomain", &Trajectory::truncate_tdomain,
      TRAJECTORY_TRAJECTORY_TRUNCATE_TDOMAIN_INTERVAL,
      "tdomain"_a)

    .def("shift_tdomain", &Trajectory::shift_tdomain,
      TRAJECTORY_TRAJECTORY_SHIFT_TDOMAIN_DOUBLE,
      "a"_a)

    .def("sample", (Trajectory & (Trajectory::*)(double))&Trajectory::sample,
        TRAJECTORY_TRAJECTORY_SAMPLE_DOUBLE,
        "timestep"_a)

    .def("sample", (Trajectory & (Trajectory::*)(const Trajectory&))&Trajectory::sample,
        TRAJECTORY_TRAJECTORY_SAMPLE_TRAJECTORY,
        "x"_a)

    .def("make_continuous", (Trajectory & (Trajectory::*)())&Trajectory::make_continuous,
        TRAJECTORY_TRAJECTORY_MAKE_CONTINUOUS)

  // Integration

    .def("primitive", (const Trajectory (Trajectory::*)(double) const)&Trajectory::primitive,
        TRAJECTORY_CONSTTRAJECTORY_PRIMITIVE_DOUBLE,
        "c"_a=0)

    .def("primitive", (const Trajectory (Trajectory::*)(double,double) const)&Trajectory::primitive,
        TRAJECTORY_CONSTTRAJECTORY_PRIMITIVE_DOUBLE_DOUBLE,
        "c"_a, "timestep"_a)

    .def("diff", &Trajectory::diff,
      TRAJECTORY_CONSTTRAJECTORY_DIFF)

    .def("finite_diff", &Trajectory::finite_diff,
        TRAJECTORY_DOUBLE_FINITE_DIFF_DOUBLE,
        "t"_a)

  // Assignments operators

    .def("__iadd__", [](Trajectory& s,double o) { return s += o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORADDEQ_DOUBLE)

    .def("__iadd__", [](Trajectory& s,const Trajectory& o) { return s += o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORADDEQ_TRAJECTORY)

    .def("__isub__", [](Trajectory& s,double o) { return s -= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORMINEQ_DOUBLE)

    .def("__isub__", [](Trajectory& s,const Trajectory& o) { return s -= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORMINEQ_TRAJECTORY)

    .def("__imul__", [](Trajectory& s,double o) { return s *= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORMULEQ_DOUBLE)

    .def("__imul__", [](Trajectory& s,const Trajectory& o) { return s *= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORMULEQ_TRAJECTORY)

    .def("__itruediv__", [](Trajectory& s,double o) { return s /= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORDIVEQ_DOUBLE)

    .def("__itruediv__", [](Trajectory& s,const Trajectory& o) { return s /= o; },
      TRAJECTORY_CONSTTRAJECTORY_OPERATORDIVEQ_TRAJECTORY)

  // String

    .def("class_name", &Trajectory::class_name,
      TRAJECTORY_CONSTSTRING_CLASS_NAME)

    .def("__repr__", [](const Trajectory& x) { ostringstream str; str << x; return str.str(); })
  
  // Operators

    .def("__add__",      [](const Trajectory& x) { return +x; })
    .def("__add__",      [](const Trajectory& x, const Trajectory& y) { return x+y; })
    .def("__add__",      [](const Trajectory& x, double y) { return x+y; })

    .def("__radd__",     [](const Trajectory& y, double x) { return x+y; })

    .def("__neg__",      [](const Trajectory& x) { return -x; })

    .def("__sub__",      [](const Trajectory& x, const Trajectory& y) { return x-y; })
    .def("__sub__",      [](const Trajectory& x, double y) { return x-y; })

    .def("__rsub__",     [](const Trajectory& y, double x) { return x-y; })

    .def("__mul__",      [](const Trajectory& x, const Trajectory& y) { return x*y; })
    .def("__mul__",      [](const Trajectory& x, double y) { return x*y; })

    .def("__rmul__",     [](const Trajectory& y, double x) { return x*y; })

    .def("__truediv__",  [](const Trajectory& x, const Trajectory& y) { return x/y; })
    .def("__truediv__",  [](const Trajectory& x, double y) { return x/y; })

    .def("__rtruediv__", [](const Trajectory& y, double x) { return x/y; })
  ;
}
