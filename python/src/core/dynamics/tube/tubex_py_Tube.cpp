/** 
 *  \file
 *  Tube Python binding
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

#include "tubex_Tube.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_Tube_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Tube(py::module& m)
{
  py::class_<Tube> tube(m, "Tube", "todo");
  tube

  // Definition

    .def(py::init<const Interval&,const Interval &>(),
      TUBE_TUBE_INTERVAL_INTERVAL,
      "tdomain"_a, "codomain"_a=Interval::all_reals())

    .def(py::init<const Interval&,double,const Interval &>(),
      TUBE_TUBE_INTERVAL_DOUBLE_INTERVAL,
      "tdomain"_a, "timestep"_a, "codomain"_a=Interval::all_reals())

    .def(py::init<const Tube&,const Interval &>(),
      TUBE_TUBE_TUBE_INTERVAL,
      "x"_a, "codomain"_a)

    .def(py::init<const Interval&,double,const TFnc&,int>(),
      TUBE_TUBE_INTERVAL_DOUBLE_TFNC_INT,
      "tdomain"_a, "timestep"_a, "f"_a, "f_image_id"_a=0)

    .def(py::init<const vector<Interval>&,const vector<Interval> &>(),
      TUBE_TUBE_VECTORINTERVAL_VECTORINTERVAL,
      "v_tdomains"_a, "v_codomains"_a)

    .def(py::init<const Tube &>(),
      TUBE_TUBE_TUBE,
      "x"_a)

    .def(py::init<const Tube&,const TFnc&,int>(),
      TUBE_TUBE_TUBE_TFNC_INT,
      "x"_a, "f"_a, "f_image_id"_a=0)

    .def(py::init<const Trajectory&,double>(),
      TUBE_TUBE_TRAJECTORY_DOUBLE,
      "traj"_a, "timestep"_a)

    .def(py::init<const Trajectory&,const Trajectory&,double>(),
      TUBE_TUBE_TRAJECTORY_TRAJECTORY_DOUBLE,
      "lb"_a, "ub"_a, "timestep"_a)

    .def(py::init<const string &>(),
      TUBE_TUBE_STRING,
      "binary_file_name"_a)

    //.def(py::init<const string&,Trajectory * &>(),
    //  TUBE_TUBE_STRING_TRAJECTORY,
    //  "binary_file_name"_a, "traj"_a)
    
    .def("size", &Tube::size,
      TUBE_INT_SIZE)

    .def("primitive", &Tube::primitive,
      TUBE_CONSTTUBE_PRIMITIVE_INTERVAL,
      "c"_a=Interval(0))

    .def("tdomain", &Tube::tdomain,
      TUBE_CONSTINTERVAL_TDOMAIN)

    .def("polygon_envelope", &Tube::polygon_envelope,
      TUBE_CONSTPOLYGON_POLYGON_ENVELOPE)

  // Slices structure

    .def("nb_slices", &Tube::nb_slices,
      TUBE_INT_NB_SLICES)

    // Note: const overloaded methods are not necessary for Python binding

    .def("slice", (Slice * (Tube::*)(int))&Tube::slice,
      TUBE_SLICE_SLICE_INT,
      "slice_id"_a, py::return_value_policy::reference_internal)

    .def("slice", (Slice * (Tube::*)(double))&Tube::slice,
      TUBE_SLICE_SLICE_DOUBLE,
      "t"_a, py::return_value_policy::reference_internal)

    .def("first_slice",(Slice * (Tube::*)())&Tube::first_slice,
      TUBE_SLICE_FIRST_SLICE,
      py::return_value_policy::reference_internal)

    .def("last_slice", (Slice * (Tube::*)())&Tube::last_slice,
      TUBE_SLICE_LAST_SLICE,
      py::return_value_policy::reference_internal)

    .def("wider_slice", (Slice * (Tube::*)())&Tube::wider_slice,
      TUBE_SLICE_WIDER_SLICE,
      py::return_value_policy::reference_internal)

    .def("largest_slice", (Slice * (Tube::*)())&Tube::largest_slice,
      TUBE_SLICE_LARGEST_SLICE,
      py::return_value_policy::reference_internal)

    .def("slice_tdomain", &Tube::slice_tdomain,
      TUBE_CONSTINTERVAL_SLICE_TDOMAIN_INT,
      "slice_id"_a)

    .def("time_to_index", &Tube::time_to_index,
      TUBE_INT_TIME_TO_INDEX_DOUBLE,
      "t"_a)

    .def("index", &Tube::index,
      TUBE_INT_INDEX_SLICE,
      "slice"_a)

    .def("sample", (void (Tube::*)(double))&Tube::sample,
      TUBE_VOID_SAMPLE_DOUBLE,
      "t"_a)

    .def("sample", (void (Tube::*)(double,Slice *))&Tube::sample,
      TUBE_VOID_SAMPLE_DOUBLE_SLICE,
      "t"_a, "slice_to_be_sampled"_a)

    .def("sample", (void (Tube::*)(double,const Interval&))&Tube::sample,
      TUBE_VOID_SAMPLE_DOUBLE_INTERVAL,
      "t"_a, "gate"_a)

    .def("sample", (void (Tube::*)(const Tube&))&Tube::sample,
      TUBE_VOID_SAMPLE_TUBE,
      "x"_a)

    .def("gate_exists", &Tube::gate_exists,
      TUBE_BOOL_GATE_EXISTS_DOUBLE,
      "t"_a)

  // Accessing values

    .def("codomain", &Tube::codomain,
      TUBE_CONSTINTERVAL_CODOMAIN)

    .def("volume", &Tube::volume,
      TUBE_DOUBLE_VOLUME)

    .def("__call__", [](Tube& s,int slice_id) { return s(slice_id); }, 
      TUBE_CONSTINTERVAL_OPERATORP_INT,
      py::return_value_policy::reference_internal)

    .def("__call__", [](Tube& s,double t) { return s(t); },
      TUBE_CONSTINTERVAL_OPERATORP_DOUBLE,
      py::return_value_policy::reference_internal)

    .def("__call__", [](Tube& s,const Interval& t) { return s(t); }, 
      TUBE_CONSTINTERVAL_OPERATORP_INTERVAL,
      py::return_value_policy::reference_internal)

    .def("eval", &Tube::eval,
      TUBE_CONSTPAIRINTERVALINTERVAL_EVAL_INTERVAL,
      "t"_a=Interval::all_reals())

    .def("interpol", (const Interval (Tube::*)(double,const Tube&) const)&Tube::interpol,
      TUBE_CONSTINTERVAL_INTERPOL_DOUBLE_TUBE,
      "t"_a, "v"_a)

    .def("interpol", (const Interval (Tube::*)(const Interval&,const Tube&) const)&Tube::interpol,
      TUBE_CONSTINTERVAL_INTERPOL_INTERVAL_TUBE,
      "t"_a, "v"_a)

    .def("invert", (const Interval (Tube::*)(const Interval&,const Interval&) const)&Tube::invert,
      TUBE_CONSTINTERVAL_INVERT_INTERVAL_INTERVAL,
      "y"_a, "search_tdomain"_a=Interval::all_reals())

    .def("invert", (void (Tube::*)(const Interval&,vector<Interval>&,const Interval&) const)&Tube::invert,
      TUBE_VOID_INVERT_INTERVAL_VECTORINTERVAL_INTERVAL,
      "y"_a, "v_t"_a, "search_tdomain"_a=Interval::all_reals())

    .def("invert", (const Interval (Tube::*)(const Interval&,const Tube&,const Interval&) const)&Tube::invert,
      TUBE_CONSTINTERVAL_INVERT_INTERVAL_TUBE_INTERVAL,
      "y"_a, "v"_a, "search_tdomain"_a=Interval::all_reals())

    .def("invert", (void (Tube::*)(const Interval&,vector<Interval>&,const Tube&,const Interval&) const)&Tube::invert,
      TUBE_VOID_INVERT_INTERVAL_VECTORINTERVAL_TUBE_INTERVAL,
      "y"_a, "v_t"_a, "v"_a, "search_tdomain"_a=Interval::all_reals())

    .def("max_diam", &Tube::max_diam,
      TUBE_DOUBLE_MAX_DIAM)

    .def("max_gate_diam", &Tube::max_gate_diam,
      TUBE_DOUBLE_MAX_GATE_DIAM_DOUBLE,
      "t"_a)

    .def("diam", (const Trajectory (Tube::*)(bool) const)&Tube::diam,
      TUBE_CONSTTRAJECTORY_DIAM_BOOL,
      "gates_thicknesses"_a=false)

    .def("diam", (const Trajectory (Tube::*)(const Tube&) const)&Tube::diam,
      TUBE_CONSTTRAJECTORY_DIAM_TUBE,
      "v"_a)

  // Tests

    .def("__eq__", [](Tube& s,const Tube& o) { return s == o; }, 
      TUBE_BOOL_OPERATOREQ_TUBE)

    .def("__ne__", [](Tube& s,const Tube& o) { return s != o; }, 
      TUBE_BOOL_OPERATORNEQ_TUBE)

    .def("is_subset", &Tube::is_subset,
      TUBE_BOOL_IS_SUBSET_TUBE,
      "x"_a)

    .def("is_strict_subset", &Tube::is_strict_subset,
      TUBE_BOOL_IS_STRICT_SUBSET_TUBE,
      "x"_a)

    .def("is_interior_subset", &Tube::is_interior_subset,
      TUBE_BOOL_IS_INTERIOR_SUBSET_TUBE,
      "x"_a)

    .def("is_strict_interior_subset", &Tube::is_strict_interior_subset,
      TUBE_BOOL_IS_STRICT_INTERIOR_SUBSET_TUBE,
      "x"_a)

    .def("is_superset", &Tube::is_superset,
      TUBE_BOOL_IS_SUPERSET_TUBE,
      "x"_a)

    .def("is_strict_superset", &Tube::is_strict_superset,
      TUBE_BOOL_IS_STRICT_SUPERSET_TUBE,
      "x"_a)

    .def("is_empty", &Tube::is_empty,
      TUBE_BOOL_IS_EMPTY)

    .def("contains", &Tube::contains,
      TUBE_CONSTBOOLINTERVAL_CONTAINS_TRAJECTORY,
      "x"_a)

    .def("overlaps", &Tube::overlaps,
      TUBE_BOOL_OVERLAPS_TUBE_FLOAT,
      "x"_a, "ratio"_a=1)

  // Setting values

    .def("set", (void (Tube::*)(const Interval&))&Tube::set,
      TUBE_VOID_SET_INTERVAL,
      "y"_a)

    .def("set", (void (Tube::*)(const Interval&,int))&Tube::set,
      TUBE_VOID_SET_INTERVAL_INT,
      "y"_a, "slice_id"_a)

    .def("set", (void (Tube::*)(const Interval&,double))&Tube::set,
      TUBE_VOID_SET_INTERVAL_DOUBLE,
      "y"_a, "t"_a)

    .def("set", (void (Tube::*)(const Interval&,const Interval&))&Tube::set,
      TUBE_VOID_SET_INTERVAL_INTERVAL,
      "y"_a, "t"_a)

    .def("set_empty", &Tube::set_empty,
      TUBE_VOID_SET_EMPTY)

    .def("inflate", (const Tube& (Tube::*)(double))&Tube::inflate,
      TUBE_CONSTTUBE_INFLATE_DOUBLE,
      "rad"_a)

    .def("inflate", (const Tube& (Tube::*)(const Trajectory&))&Tube::inflate,
      TUBE_CONSTTUBE_INFLATE_TRAJECTORY,
      "rad"_a)

    .def("shift_tdomain", &Tube::shift_tdomain,
      TUBE_VOID_SHIFT_TDOMAIN_DOUBLE,
      "a"_a)

    .def("remove_gate", &Tube::remove_gate,
      TUBE_VOID_REMOVE_GATE_DOUBLE,
      "t"_a)

  // Bisection

    .def("bisect", &Tube::bisect,
      TUBE_CONSTPAIRTUBETUBE_BISECT_DOUBLE_FLOAT,
      "t"_a, "ratio"_a=0.49)
  
  // Assignments operators

    .def("__iadd__", [](Tube& s,const Interval& o) { return s += o; },
      TUBE_CONSTTUBE_OPERATORADDEQ_INTERVAL)

    .def("__iadd__", [](Tube& s,const Trajectory& o) { return s += o; },
      TUBE_CONSTTUBE_OPERATORADDEQ_TRAJECTORY)

    .def("__iadd__", [](Tube& s,const Tube& o) { return s += o; },
      TUBE_CONSTTUBE_OPERATORADDEQ_TUBE)

    .def("__isub__", [](Tube& s,const Interval& o) { return s -= o; },
      TUBE_CONSTTUBE_OPERATORMINEQ_INTERVAL)

    .def("__isub__", [](Tube& s,const Trajectory& o) { return s -= o; },
      TUBE_CONSTTUBE_OPERATORMINEQ_TRAJECTORY)

    .def("__isub__", [](Tube& s,const Tube& o) { return s -= o; },
      TUBE_CONSTTUBE_OPERATORMINEQ_TUBE)

    .def("__imul__", [](Tube& s,const Interval& o) { return s *= o; },
      TUBE_CONSTTUBE_OPERATORMULEQ_INTERVAL)

    .def("__imul__", [](Tube& s,const Trajectory& o) { return s *= o; },
      TUBE_CONSTTUBE_OPERATORMULEQ_TRAJECTORY)

    .def("__imul__", [](Tube& s,const Tube& o) { return s *= o; },
      TUBE_CONSTTUBE_OPERATORMULEQ_TUBE)

    .def("__itruediv__", [](Tube& s,const Interval& o) { return s /= o; },
      TUBE_CONSTTUBE_OPERATORDIVEQ_INTERVAL)

    .def("__itruediv__", [](Tube& s,const Trajectory& o) { return s /= o; },
      TUBE_CONSTTUBE_OPERATORDIVEQ_TRAJECTORY)

    .def("__itruediv__", [](Tube& s,const Tube& o) { return s /= o; },
      TUBE_CONSTTUBE_OPERATORDIVEQ_TUBE)

    .def("__ior__", [](Tube& s,const Interval& o) { return s |= o; },
      TUBE_CONSTTUBE_OPERATORUNIEQ_INTERVAL)

    .def("__ior__", [](Tube& s,const Trajectory& o) { return s |= o; },
      TUBE_CONSTTUBE_OPERATORUNIEQ_TRAJECTORY)

    .def("__ior__", [](Tube& s,const Tube& o) { return s |= o; },
      TUBE_CONSTTUBE_OPERATORUNIEQ_TUBE)

    .def("__iand__", [](Tube& s,const Interval& o) { return s &= o; },
      TUBE_CONSTTUBE_OPERATORINTEQ_INTERVAL)

    .def("__iand__", [](Tube& s,const Trajectory& o) { return s &= o; },
      TUBE_CONSTTUBE_OPERATORINTEQ_TRAJECTORY)

    .def("__iand__", [](Tube& s,const Tube& o) { return s &= o; },
      TUBE_CONSTTUBE_OPERATORINTEQ_TUBE)

  // String

    .def("class_name", &Tube::class_name,
      TUBE_CONSTSTRING_CLASS_NAME)

    .def("__repr__", [](const Tube& x) { ostringstream str; str << x; return str.str(); })
  
  // Synthesis tree

    .def("enable_synthesis", &Tube::enable_synthesis,
      TUBE_VOID_ENABLE_SYNTHESIS_BOOL,
      "enable"_a=true)

  // Integration

    .def("integral", (const Interval (Tube::*)(double) const)&Tube::integral,
      TUBE_CONSTINTERVAL_INTEGRAL_DOUBLE,
      "t"_a)

    .def("integral", (const Interval (Tube::*)(const Interval&) const)&Tube::integral,
      TUBE_CONSTINTERVAL_INTEGRAL_INTERVAL,
      "t"_a)

    .def("integral", (const Interval (Tube::*)(const Interval&,const Interval&) const)&Tube::integral,
      TUBE_CONSTINTERVAL_INTEGRAL_INTERVAL_INTERVAL,
      "t1"_a, "t2"_a)

    .def("partial_integral", (const pair<Interval,Interval> (Tube::*)(const Interval&) const)&Tube::partial_integral,
      TUBE_CONSTPAIRINTERVALINTERVAL_PARTIAL_INTEGRAL_INTERVAL,
      "t"_a)

    .def("partial_integral", (const pair<Interval,Interval> (Tube::*)(const Interval&,const Interval&) const)&Tube::partial_integral,
      TUBE_CONSTPAIRINTERVALINTERVAL_PARTIAL_INTEGRAL_INTERVAL_INTERVAL,
      "t1"_a, "t2"_a)

  // Serialization

    .def("serialize", (void (Tube::*)(const string&,int) const)&Tube::serialize,
      TUBE_VOID_SERIALIZE_STRING_INT,
      "binary_file_name"_a="x.tube", "version_number"_a=SERIALIZATION_VERSION)

    .def("serialize", (void (Tube::*)(const string&,const Trajectory&,int) const)&Tube::serialize,
      TUBE_VOID_SERIALIZE_STRING_TRAJECTORY_INT,
      "binary_file_name"_a, "traj"_a, "version_number"_a=SERIALIZATION_VERSION)

  // Static methods

    .def_static("same_slicing", &Tube::same_slicing,
      TUBE_BOOL_SAME_SLICING_TUBE_TUBE,
      "x1"_a, "x2"_a)

    .def_static("enable_syntheses", &Tube::enable_syntheses,
      TUBE_VOID_ENABLE_SYNTHESES_BOOL,
      "enable"_a=true)

    .def_static("hull", &Tube::hull,
      TUBE_CONSTTUBE_HULL_LISTTUBE,
      "l_tubes"_a)

  // Operators

    .def("__add__",      [](const Tube& x) { return +x; })
    .def("__add__",      [](const Tube& x, const Tube& y) { return x+y; })
    .def("__add__",      [](const Tube& x, double y) { return x+y; })
    .def("__add__",      [](const Tube& x, const Interval& y) { return x+y; })
    .def("__add__",      [](const Tube& x, const Trajectory& y) { return x+y; })

    .def("__radd__",     [](const Tube& y, double x) { return x+y; })
    .def("__radd__",     [](const Tube& y, const Interval& x) { return x+y; })
    .def("__radd__",     [](const Tube& y, const Trajectory& x) { return x+y; })

    .def("__neg__",      [](const Tube& x) { return -x; })
    .def("__sub__",      [](const Tube& x, const Tube& y) { return x-y; })
    .def("__sub__",      [](const Tube& x, double y) { return x-y; })
    .def("__sub__",      [](const Tube& x, const Interval& y) { return x-y; })
    .def("__sub__",      [](const Tube& x, const Trajectory& y) { return x-y; })

    .def("__rsub__",     [](const Tube& y, const Interval& x) { return x-y; })
    .def("__rsub__",     [](const Tube& y, const Trajectory& x) { return x-y; })

    .def("__mul__",      [](const Tube& x, const Tube& y) { return x*y; })
    .def("__mul__",      [](const Tube& x, double y) { return x*y; })
    .def("__mul__",      [](const Tube& x, const Interval& y) { return x*y; })
    .def("__mul__",      [](const Tube& x, const Trajectory& y) { return x*y; })
    // Vector case
    .def("__mul__",      [](const Tube& x, const IntervalVector& y) { return x*y; })
    .def("__mul__",      [](const Tube& x, const TubeVector& y) { return x*y; })

    .def("__rmul__",     [](const Tube& y, double x) { return x*y; })
    .def("__rmul__",     [](const Tube& y, const Interval& x) { return x*y; })
    .def("__rmul__",     [](const Tube& y, const Trajectory& x) { return x*y; })

    .def("__truediv__",  [](const Tube& x, const Tube& y) { return x/y; })
    .def("__truediv__",  [](const Tube& x, double y) { return x/y; })
    .def("__truediv__",  [](const Tube& x, const Interval& y) { return x/y; })
    .def("__truediv__",  [](const Tube& x, const Trajectory& y) { return x/y; })

    .def("__rtruediv__", [](const Tube& y, const double x) { return x/y; })
    .def("__rtruediv__", [](const Tube& y, const Interval& x) { return x/y; })
    .def("__rtruediv__", [](const Tube& y, const Trajectory& x) { return x/y; })
    // Vector case
    .def("__rtruediv__", [](const Tube& y, const IntervalVector& x) { return x/y; })
    .def("__rtruediv__", [](const Tube& y, const TubeVector& x) { return x/y; })

    .def("__or__",       [](const Tube& x, const Tube& y) { return x|y; })
    .def("__or__",       [](const Tube& x, double y) { return x|y; })
    .def("__or__",       [](const Tube& x, const Interval& y) { return x|y; })
    .def("__or__",       [](const Tube& x, const Trajectory& y) { return x|y; })

    .def("__ror__",      [](const Tube& y, double x) { return x|y; })
    .def("__ror__",      [](const Tube& y, const Interval& x) { return x|y; })
    .def("__ror__",      [](const Tube& y, const Trajectory& x) { return x|y; })

    .def("__and__",      [](const Tube& x, const Tube& y) { return x&y; })
    .def("__and__",      [](const Tube& x, double y) { return x&y; })
    .def("__and__",      [](const Tube& x, const Interval& y) { return x&y; })
    .def("__and__",      [](const Tube& x, const Trajectory& y) { return x&y; })

    .def("__rand__",     [](const Tube& y, double x) { return x&y; })
    .def("__rand__",     [](const Tube& y, const Interval& x) { return x&y; })
    .def("__rand__",     [](const Tube& y, const Trajectory& x) { return x&y; })
  ;
}