/** 
 *  \file
 *  Tube binding
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

#include "tubex_Tube.h"
#include "tubex_py_Tube_docs.h"
#include "ibex_Interval.h"

using namespace ibex;
using namespace tubex;

using namespace pybind11::literals;
namespace py = pybind11;
using py::class_;
using py::init;


std::string to_string(const Tube& x)
{
  std::ostringstream str;
  str << x;
  return str.str();
}

void export_Tube(py::module& m)
{
  py::class_<Tube> tube(m, "Tube", DOCS_TUBE);
  tube

  // Definition

    .def(py::init<const Interval &,const Interval &>(),
      DOCS_TUBE_TUBE_INTERVAL_INTERVAL,
      "tdomain"_a, "codomain"_a=Interval::all_reals())

    .def(py::init<const Interval &,double,const Interval &>(),
      DOCS_TUBE_TUBE_INTERVAL_DOUBLE_INTERVAL,
      "tdomain"_a, "timestep"_a, "codomain"_a=Interval::all_reals())

    .def(py::init<const Tube &,const Interval &>(),
      DOCS_TUBE_TUBE_TUBE_INTERVAL,
      "x"_a, "codomain"_a)

    .def(py::init<const Interval &,double,const TFnc &,int>(),
      DOCS_TUBE_TUBE_INTERVAL_DOUBLE_FNC_INT,
      "tdomain"_a, "timestep"_a, "f"_a, "f_image_id"_a=0)

    .def(py::init<const std::vector<Interval> &,const std::vector<Interval> &>(),
      DOCS_TUBE_TUBE_VECTOR_INTERVAL__VECTOR_INTERVAL_,
      "v_domains"_a, "v_codomains"_a)

    .def(py::init<const Tube &>(),
      DOCS_TUBE_TUBE_TUBE,
      "x"_a)

    .def(py::init<const Tube &,const TFnc &,int>(),
      DOCS_TUBE_TUBE_TUBE_FNC_INT,
      "x"_a, "f"_a, "f_image_id"_a=0)

    .def(py::init<const Trajectory &,double>(),
      DOCS_TUBE_TUBE_TRAJECTORY_DOUBLE,
      "traj"_a, "timestep"_a)

    .def(py::init<const Trajectory &,const Trajectory &,double>(),
      DOCS_TUBE_TUBE_TRAJECTORY_TRAJECTORY_DOUBLE,
      "lb"_a, "ub"_a, "timestep"_a)

    .def(py::init<const std::string &>(),
      DOCS_TUBE_TUBE_STRING,
      "binary_file_name"_a)

    //.def(py::init<const std::string &,Trajectory * &>(),
    //  DOCS_TUBE_TUBE_STRING_TRAJECTORY,
    //  "binary_file_name"_a, "traj"_a)
    
    .def("size", &Tube::size,
      DOCS_TUBE_SIZE)

    .def("primitive", &Tube::primitive,
      DOCS_TUBE_PRIMITIVE_INTERVAL,
      "c"_a=Interval(0))

    .def("tdomain", &Tube::tdomain,
      DOCS_TUBE_TDOMAIN)

    .def("polygon_envelope", &Tube::polygon_envelope,
      DOCS_TUBE_POLYGON_ENVELOPE)

  // Slices structure

    .def("nb_slices", &Tube::nb_slices,
      DOCS_TUBE_NB_SLICES)

    .def("slice", (Slice * (Tube::*)(int))&Tube::slice,
      DOCS_TUBE_SLICE_INT,
      "slice_id"_a, py::return_value_policy::reference_internal)

    .def("slice", (const Slice * (Tube::*)(int) const)&Tube::slice,
      DOCS_TUBE_SLICE_INT1,
      "slice_id"_a,py::return_value_policy::reference_internal)

    .def("slice", (Slice * (Tube::*)(double))&Tube::slice,
      DOCS_TUBE_SLICE_DOUBLE,
      "t"_a, py::return_value_policy::reference_internal)

    .def("slice", (const Slice * (Tube::*)(double) const)&Tube::slice,
      DOCS_TUBE_SLICE_DOUBLE1,
      "t"_a, py::return_value_policy::reference_internal)

    .def("first_slice",(Slice * (Tube::*)())&Tube::first_slice,
      DOCS_TUBE_FIRST_SLICE,
      py::return_value_policy::reference_internal)

    .def("first_slice", (const Slice * (Tube::*)() const)&Tube::first_slice,
      DOCS_TUBE_FIRST_SLICE1,
      py::return_value_policy::reference_internal)

    .def("last_slice", (Slice * (Tube::*)())&Tube::last_slice,
      DOCS_TUBE_LAST_SLICE,
      py::return_value_policy::reference_internal)

    .def("last_slice", (const Slice * (Tube::*)() const)&Tube::last_slice,
      DOCS_TUBE_LAST_SLICE1,
      py::return_value_policy::reference_internal)

    .def("wider_slice", (Slice * (Tube::*)())&Tube::wider_slice,
      DOCS_TUBE_WIDER_SLICE,
      py::return_value_policy::reference_internal)

    .def("wider_slice", (const Slice * (Tube::*)() const)&Tube::wider_slice,
      DOCS_TUBE_WIDER_SLICE1,
      py::return_value_policy::reference_internal)

    .def("largest_slice", (Slice * (Tube::*)())&Tube::largest_slice,
      DOCS_TUBE_LARGEST_SLICE,
      py::return_value_policy::reference_internal)

    .def("largest_slice", (const Slice * (Tube::*)() const)&Tube::largest_slice,
      DOCS_TUBE_LARGEST_SLICE1,
      py::return_value_policy::reference_internal)

    .def("slice_tdomain", &Tube::slice_tdomain,
      DOCS_TUBE_SLICE_TDOMAIN_INT,
      "slice_id"_a)

    .def("input2index", &Tube::input2index,
      DOCS_TUBE_INPUT2INDEX_DOUBLE,
      "t"_a)

    .def("index", &Tube::index,
      DOCS_TUBE_INDEX_SLICE,
      "slice"_a)

    .def("sample", (void (Tube::*)(double))&Tube::sample,
      DOCS_TUBE_SAMPLE_DOUBLE,
      "t"_a)

    .def("sample", (void (Tube::*)(double,Slice *))&Tube::sample,
      DOCS_TUBE_SAMPLE_DOUBLE_SLICE,
      "t"_a, "slice_to_be_sampled"_a)

    .def("sample", (void (Tube::*)(double,const Interval &))&Tube::sample,
      DOCS_TUBE_SAMPLE_DOUBLE_INTERVAL,
      "t"_a, "gate"_a)

    .def("sample", (void (Tube::*)(const Tube &))&Tube::sample,
      DOCS_TUBE_SAMPLE_TUBE,
      "x"_a)

    .def("gate_exists", &Tube::gate_exists,
      DOCS_TUBE_GATE_EXISTS_DOUBLE,
      "t"_a)

  // Accessing values

    .def("codomain", &Tube::codomain,
      DOCS_TUBE_CODOMAIN)

    .def("volume", &Tube::volume,
      DOCS_TUBE_VOLUME)

    .def("__call__", [](Tube& s,int slice_id) { return s(slice_id); }, 
      DOCS_TUBE_OPERATOR_CALL_INT,
      py::return_value_policy::reference_internal)

    .def("__call__", [](Tube& s,double t) { return s(t); },
      DOCS_TUBE_OPERATOR_CALL_DOUBLE,
      py::return_value_policy::reference_internal)

    .def("__call__", [](Tube& s,const Interval & t) { return s(t); }, 
      DOCS_TUBE_OPERATOR_CALL_INTERVAL,
      py::return_value_policy::reference_internal)

    .def("eval", &Tube::eval,
      DOCS_TUBE_EVAL_INTERVAL,
      "t"_a=Interval::all_reals())

    .def("interpol", (const Interval (Tube::*)(double,const Tube &) const)&Tube::interpol,
      DOCS_TUBE_INTERPOL_DOUBLE_TUBE,
      "t"_a, "v"_a)

    .def("interpol", (const Interval (Tube::*)(const Interval &,const Tube &) const)&Tube::interpol,
      DOCS_TUBE_INTERPOL_INTERVAL_TUBE,
      "t"_a, "v"_a)

    .def("invert", (const Interval (Tube::*)(const Interval &,const Interval &) const)&Tube::invert,
      DOCS_TUBE_INVERT_INTERVAL_INTERVAL,
      "y"_a, "search_tdomain"_a=Interval::all_reals())

    .def("invert", (void (Tube::*)(const Interval &,std::vector<Interval> &,const Interval &) const)&Tube::invert,
      DOCS_TUBE_INVERT_INTERVAL_VECTOR_INTERVAL__INTERVAL,
      "y"_a, "v_t"_a, "search_tdomain"_a=Interval::all_reals())

    .def("invert", (const Interval (Tube::*)(const Interval &,const Tube &,const Interval &) const)&Tube::invert,
      DOCS_TUBE_INVERT_INTERVAL_TUBE_INTERVAL,
      "y"_a, "v"_a, "search_tdomain"_a=Interval::all_reals())

    .def("invert", (void (Tube::*)(const Interval &,std::vector<Interval> &,const Tube &,const Interval &) const)&Tube::invert,
      DOCS_TUBE_INVERT_INTERVAL_VECTOR_INTERVAL__TUBE_INTERVAL,
      "y"_a, "v_t"_a, "v"_a, "search_tdomain"_a=Interval::all_reals())

    .def("max_diam", &Tube::max_diam,
      DOCS_TUBE_MAX_DIAM)

    .def("max_gate_diam", &Tube::max_gate_diam,
      DOCS_TUBE_MAX_GATE_DIAM_DOUBLE,
      "t"_a)

    .def("diam", (const Trajectory (Tube::*)(bool) const)&Tube::diam,
      DOCS_TUBE_DIAM_BOOL,
      "gates_thicknesses"_a=false)

    .def("diam", (const Trajectory (Tube::*)(const Tube &) const)&Tube::diam,
      DOCS_TUBE_DIAM_TUBE,
      "v"_a)

  // Tests

    .def("__eq__", [](Tube& s,const Tube & o) { return s == o; }, 
      DOCS_TUBE_EQ_TUBE)

    .def("__ne__", [](Tube& s,const Tube & o) { return s != o; }, 
      DOCS_TUBE_NE_TUBE)

    .def("is_subset", &Tube::is_subset,
      DOCS_TUBE_IS_SUBSET_TUBE,
      "x"_a)

    .def("is_strict_subset", &Tube::is_strict_subset,
      DOCS_TUBE_IS_STRICT_SUBSET_TUBE,
      "x"_a)

    .def("is_interior_subset", &Tube::is_interior_subset,
      DOCS_TUBE_IS_INTERIOR_SUBSET_TUBE,
      "x"_a)

    .def("is_strict_interior_subset", &Tube::is_strict_interior_subset,
      DOCS_TUBE_IS_STRICT_INTERIOR_SUBSET_TUBE,
      "x"_a)

    .def("is_superset", &Tube::is_superset,
      DOCS_TUBE_IS_SUPERSET_TUBE,
      "x"_a)

    .def("is_strict_superset", &Tube::is_strict_superset,
      DOCS_TUBE_IS_STRICT_SUPERSET_TUBE,
      "x"_a)

    .def("is_empty", &Tube::is_empty,
      DOCS_TUBE_IS_EMPTY)

    .def("contains", &Tube::contains,
      DOCS_TUBE_CONTAINS_TRAJECTORY,
      "x"_a)

    .def("overlaps", &Tube::overlaps,
      DOCS_TUBE_OVERLAPS_TUBE_FLOAT,
      "x"_a, "ratio"_a=1)

  // Setting values

    .def("set", (void (Tube::*)(const Interval &))&Tube::set,
      DOCS_TUBE_SET_INTERVAL,
      "y"_a)

    .def("set", (void (Tube::*)(const Interval &,int))&Tube::set,
      DOCS_TUBE_SET_INTERVAL_INT,
      "y"_a, "slice_id"_a)

    .def("set", (void (Tube::*)(const Interval &,double))&Tube::set,
      DOCS_TUBE_SET_INTERVAL_DOUBLE,
      "y"_a, "t"_a)

    .def("set", (void (Tube::*)(const Interval &,const Interval &))&Tube::set,
      DOCS_TUBE_SET_INTERVAL_INTERVAL,
      "y"_a, "t"_a)

    .def("set_empty", &Tube::set_empty,
      DOCS_TUBE_SET_EMPTY)

    .def("inflate", (const Tube & (Tube::*)(double))&Tube::inflate,
      DOCS_TUBE_INFLATE_DOUBLE,
      "rad"_a)

    .def("inflate", (const Tube & (Tube::*)(const Trajectory &))&Tube::inflate,
      DOCS_TUBE_INFLATE_TRAJECTORY,
      "rad"_a)

    .def("shift_tdomain", &Tube::shift_tdomain,
      DOCS_TUBE_SHIFT_TDOMAIN_DOUBLE,
      "a"_a)

    .def("remove_gate", &Tube::remove_gate,
      DOCS_TUBE_REMOVE_GATE_DOUBLE,
      "t"_a)

  // Bisection

    .def("bisect", &Tube::bisect,
      DOCS_TUBE_BISECT_DOUBLE_FLOAT,
      "t"_a, "ratio"_a=0.49)
  
  // Assignments operators

    .def("__iadd__", [](Tube& s,const Interval & o) { return s += o; },
      DOCS_TUBE_IADD_INTERVAL)

    .def("__iadd__", [](Tube& s,const Trajectory & o) { return s += o; },
      DOCS_TUBE_IADD_TRAJECTORY)

    .def("__iadd__", [](Tube& s,const Tube & o) { return s += o; },
      DOCS_TUBE_IADD_TUBE)

    .def("__isub__", [](Tube& s,const Interval & o) { return s -= o; },
      DOCS_TUBE_ISUB_INTERVAL)

    .def("__isub__", [](Tube& s,const Trajectory & o) { return s -= o; },
      DOCS_TUBE_ISUB_TRAJECTORY)

    .def("__isub__", [](Tube& s,const Tube & o) { return s -= o; },
      DOCS_TUBE_ISUB_TUBE)

    .def("__imul__", [](Tube& s,const Interval & o) { return s *= o; },
      DOCS_TUBE_IMUL_INTERVAL)

    .def("__imul__", [](Tube& s,const Trajectory & o) { return s *= o; },
      DOCS_TUBE_IMUL_TRAJECTORY)

    .def("__imul__", [](Tube& s,const Tube & o) { return s *= o; },
      DOCS_TUBE_IMUL_TUBE)

    .def("__itruediv__", [](Tube& s,const Interval & o) { return s /= o; },
      DOCS_TUBE_ITRUEDIV_INTERVAL)

    .def("__itruediv__", [](Tube& s,const Trajectory & o) { return s /= o; },
      DOCS_TUBE_ITRUEDIV_TRAJECTORY)

    .def("__itruediv__", [](Tube& s,const Tube & o) { return s /= o; },
      DOCS_TUBE_ITRUEDIV_TUBE)

    .def("__ior__", [](Tube& s,const Interval & o) { return s |= o; },
      DOCS_TUBE_IOR_INTERVAL)

    .def("__ior__", [](Tube& s,const Trajectory & o) { return s |= o; },
      DOCS_TUBE_IOR_TRAJECTORY)

    .def("__ior__", [](Tube& s,const Tube & o) { return s |= o; },
      DOCS_TUBE_IOR_TUBE)

    .def("__iand__", [](Tube& s,const Interval & o) { return s &= o; },
      DOCS_TUBE_IAND_INTERVAL)

    .def("__iand__", [](Tube& s,const Trajectory & o) { return s &= o; },
      DOCS_TUBE_IAND_TRAJECTORY)

    .def("__iand__", [](Tube& s,const Tube & o) { return s &= o; },
      DOCS_TUBE_IAND_TUBE)

  // String

    .def("class_name", &Tube::class_name,
      DOCS_TUBE_CLASS_NAME)

    .def("__repr__", &to_string)
  
  // Synthesis tree

    .def("enable_synthesis", &Tube::enable_synthesis,
      DOCS_TUBE_ENABLE_SYNTHESIS_BOOL,
      "enable"_a=true)

  // Integration

    .def("integral", (const Interval (Tube::*)(double) const)&Tube::integral,
      DOCS_TUBE_INTEGRAL_DOUBLE,
      "t"_a)

    .def("integral", (const Interval (Tube::*)(const Interval &) const)&Tube::integral,
      DOCS_TUBE_INTEGRAL_INTERVAL,
      "t"_a)

    .def("integral", (const Interval (Tube::*)(const Interval &,const Interval &) const)&Tube::integral,
      DOCS_TUBE_INTEGRAL_INTERVAL_INTERVAL,
      "t1"_a, "t2"_a)

    .def("partial_integral", (const std::pair<Interval, Interval> (Tube::*)(const Interval &) const)&Tube::partial_integral,
      DOCS_TUBE_PARTIAL_INTEGRAL_INTERVAL,
      "t"_a)

    .def("partial_integral", (const std::pair<Interval, Interval> (Tube::*)(const Interval &,const Interval &) const)&Tube::partial_integral,
      DOCS_TUBE_PARTIAL_INTEGRAL_INTERVAL_INTERVAL,
      "t1"_a, "t2"_a)

  // Serialization

    .def("serialize", (void (Tube::*)(const std::string &,int) const)&Tube::serialize,
      DOCS_TUBE_SERIALIZE_STRING_INT,
      "binary_file_name"_a="x.tube", "version_number"_a=SERIALIZATION_VERSION)

    .def("serialize", (void (Tube::*)(const std::string &,const Trajectory &,int) const)&Tube::serialize,
      DOCS_TUBE_SERIALIZE_STRING_TRAJECTORY_INT,
      "binary_file_name"_a, "traj"_a, "version_number"_a=SERIALIZATION_VERSION)

  // Static methods

    .def_static("same_slicing", &Tube::same_slicing,
      DOCS_TUBE_SAME_SLICING_TUBE_TUBE,
      "x1"_a, "x2"_a)

    .def_static("enable_syntheses", &Tube::enable_syntheses,
      DOCS_TUBE_ENABLE_SYNTHESES_BOOL,
      "enable"_a=true)

    .def_static("hull", &Tube::hull,
      DOCS_TUBE_HULL_LIST_TUBE_,
      "l_tubes"_a)

  // Operators

    .def("__add__",      [](const Tube& x) { return +x; })
    .def("__add__",      [](const Tube& x, const Tube& y) { return x+y; })
    .def("__add__",      [](const Tube& x, const Interval& y) { return x+y; })
    .def("__radd__",     [](const Tube& x, const Interval& y) { return x+y; })
    .def("__add__",      [](const Tube& x, const Trajectory& y) { return x+y; })
    .def("__radd__",     [](const Tube& x, const Trajectory& y) { return x+y; })

    .def("__neg__",      [](const Tube& x) { return -x; })
    .def("__sub__",      [](const Tube& x, const Tube& y) { return x-y; })
    .def("__sub__",      [](const Tube& x, const Interval& y) { return x-y; })
    .def("__rsub__",     [](const Tube& x, const Interval& y) { return y-x; })
    .def("__sub__",      [](const Tube& x, const Trajectory& y) { return x-y; })
    .def("__rsub__",     [](const Tube& x, const Trajectory& y) { return y-x; })

    .def("__mul__",      [](const Tube& x, const Tube& y) { return x*y; }, "y"_a.noconvert())
    .def("__mul__",      [](const Tube& x, const Interval& y) { return x*y; }, "y"_a.noconvert())
    .def("__rmul__",     [](const Tube& x, const Interval& y) { return x*y; }, "y"_a.noconvert())
    .def("__mul__",      [](const Tube& x, const Trajectory& y) { return x*y; }, "y"_a.noconvert())
    .def("__rmul__",     [](const Tube& x, const Trajectory& y) { return x*y; }, "y"_a.noconvert())
    // Vector case
    .def("__mul__",      [](const Tube& x, const ibex::IntervalVector& y) { return x*y; }, "y"_a.noconvert())
    .def("__mul__",      [](const Tube& x, const TubeVector& y) { return x*y; }, "y"_a.noconvert())

    .def("__truediv__",  [](const Tube& x, const Tube& y) { return x/y; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    .def("__truediv__",  [](const Tube& x, const Interval& y) { return x/y; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    .def("__rtruediv__", [](const Tube& x, const Interval& y) { return y/x; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    .def("__truediv__",  [](const Tube& x, const Trajectory& y) { return x/y; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    .def("__rtruediv__", [](const Tube& x, const Trajectory& y) { return y/x; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    // Vector case
    .def("__rtruediv__", [](const Tube& x, const IntervalVector& y) { return y/x; }) //, "x"_a.noconvert(), "y"_a.noconvert())
    .def("__rtruediv__", [](const Tube& x, const TubeVector& y) { return y/x; }) //, "x"_a.noconvert(), "y"_a.noconvert())

    .def("__or__",       [](const Tube& x, const Tube& y) { return x|y; })
    .def("__or__",       [](const Tube& x, const Interval& y) { return x|y; })
    .def("__ror__",      [](const Tube& x, const Interval& y) { return x|y; })
    .def("__or__",       [](const Tube& x, const Trajectory& y) { return x|y; })
    .def("__ror__",      [](const Tube& x, const Trajectory& y) { return x|y; })

    .def("__and__",      [](const Tube& x, const Tube& y) { return x & y; })
    .def("__and__",      [](const Tube& x, const Interval& y) { return x & y; })
    .def("__rand__",     [](const Tube& x, const Interval& y) { return x & y; })
    .def("__and__",      [](const Tube& x, const Trajectory& y) { return x & y; })
    .def("__rand__",     [](const Tube& x, const Trajectory& y) { return x & y; })
  ;
}