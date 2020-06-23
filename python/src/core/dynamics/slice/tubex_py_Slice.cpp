/** 
 *  \file
 *  Slice Python binding
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

#include "tubex_Slice.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_Slice_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Slice(py::module& m)
{
  py::class_<Slice> slice(m, "Slice", SLICE_MAIN);
  slice

  // Definition

    .def(py::init<const Interval&,const Interval&>(),
      SLICE_SLICE_INTERVAL_INTERVAL,
      "tdomain"_a, "codomain"_a=Interval::ALL_REALS)

    .def(py::init<const Slice&>(),
      SLICE_SLICE_SLICE,
      "x"_a)

    .def("size", &Slice::size,
      SLICE_INT_SIZE)

    .def("tdomain", &Slice::tdomain,
      SLICE_CONSTINTERVAL_TDOMAIN)
    
  // Slices structure

    .def("prev_slice", (Slice * (Slice::*)())&Slice::prev_slice,
      SLICE_SLICE_PREV_SLICE)

    .def("next_slice", (Slice * (Slice::*)())&Slice::next_slice,
      SLICE_SLICE_NEXT_SLICE)

    .def("input_gate", &Slice::input_gate,
      SLICE_CONSTINTERVAL_INPUT_GATE)

    .def("output_gate", &Slice::output_gate,
      SLICE_CONSTINTERVAL_OUTPUT_GATE)

    .def("polygon", &Slice::polygon,
      SLICE_CONSTCONVEXPOLYGON_POLYGON_SLICE,
      "v"_a)
  
  // Accessing values

    .def("codomain", &Slice::codomain,
      SLICE_CONSTINTERVAL_CODOMAIN)

    .def("box", &Slice::box,
      SLICE_CONSTINTERVALVECTOR_BOX)

    .def("diam", (double (Slice::*)() const)&Slice::diam,
      SLICE_DOUBLE_DIAM)

    .def("volume", &Slice::volume,
      SLICE_DOUBLE_VOLUME)

    .def("__call__", [](Slice& s,double o) { return s(o);}, 
      SLICE_CONSTINTERVAL_OPERATORP_DOUBLE)

    .def("__call__", [](Slice& s,const Interval& o) { return s(o);}, 
      SLICE_CONSTINTERVAL_OPERATORP_INTERVAL)

    .def("eval", &Slice::eval,
      SLICE_CONSTPAIRINTERVALINTERVAL_EVAL_INTERVAL,
      "t"_a=Interval::ALL_REALS)

    .def("interpol", (const Interval (Slice::*)(double,const Slice&) const)&Slice::interpol,
      SLICE_CONSTINTERVAL_INTERPOL_DOUBLE_SLICE,
      "t"_a, "v"_a)

    .def("interpol", (const Interval (Slice::*)(const Interval&,const Slice&) const)&Slice::interpol,
      SLICE_CONSTINTERVAL_INTERPOL_INTERVAL_SLICE,
      "t"_a, "v"_a)

    .def("invert", (const Interval (Slice::*)(const Interval&,const Interval&) const)&Slice::invert,
      SLICE_CONSTINTERVAL_INVERT_INTERVAL_INTERVAL,
      "y"_a, "search_tdomain"_a=Interval::ALL_REALS)

    .def("invert", (const Interval (Slice::*)(const Interval&,const Slice&,const Interval&) const)&Slice::invert,
      SLICE_CONSTINTERVAL_INVERT_INTERVAL_SLICE_INTERVAL,
      "y"_a, "v"_a, "search_tdomain"_a=Interval::ALL_REALS)
  
  // Tests

    .def("__eq__", [](Slice& s,const Slice& o) { return s == o;}, 
      SLICE_BOOL_OPERATOREQ_SLICE)

    .def("__ne__", [](Slice& s,const Slice& o) { return s != o;}, 
      SLICE_BOOL_OPERATORNEQ_SLICE)

    .def("is_subset", &Slice::is_subset,
      SLICE_BOOL_IS_SUBSET_SLICE,
      "x"_a)

    .def("is_strict_subset", &Slice::is_strict_subset,
      SLICE_BOOL_IS_STRICT_SUBSET_SLICE,
      "x"_a)

    .def("is_interior_subset", &Slice::is_interior_subset,
      SLICE_BOOL_IS_INTERIOR_SUBSET_SLICE,
      "x"_a)

    .def("is_strict_interior_subset", &Slice::is_strict_interior_subset,
      SLICE_BOOL_IS_STRICT_INTERIOR_SUBSET_SLICE,
      "x"_a)

    .def("is_superset", &Slice::is_superset,
      SLICE_BOOL_IS_SUPERSET_SLICE,
      "x"_a)

    .def("is_strict_superset", &Slice::is_strict_superset,
      SLICE_BOOL_IS_STRICT_SUPERSET_SLICE,
      "x"_a)

    .def("is_empty", &Slice::is_empty,
      SLICE_BOOL_IS_EMPTY)

    .def("contains", &Slice::contains,
      SLICE_CONSTBOOLINTERVAL_CONTAINS_TRAJECTORY,
      "x"_a)
  
  // Setting values

    .def("set", &Slice::set,
      SLICE_VOID_SET_INTERVAL,
      "y"_a)

    .def("set_empty", &Slice::set_empty,
      SLICE_VOID_SET_EMPTY)

    .def("set_envelope", &Slice::set_envelope,
      SLICE_VOID_SET_ENVELOPE_INTERVAL_BOOL, 
      "envelope"_a, "slice_consistency"_a=true)

    .def("set_input_gate", &Slice::set_input_gate,
      SLICE_VOID_SET_INPUT_GATE_INTERVAL_BOOL,
      "input_gate"_a, "slice_consistency"_a=true)

    .def("set_output_gate", &Slice::set_output_gate,
      SLICE_VOID_SET_OUTPUT_GATE_INTERVAL_BOOL,
      "output_gate"_a, "slice_consistency"_a=true)

    .def("inflate", &Slice::inflate,
      SLICE_CONSTSLICE_INFLATE_DOUBLE,
      "rad"_a)
  
  // Assignments operators

    .def("__iadd__", [](Slice& s,double o) { return s += o;}, 
      SLICE_SLICE_OPERATORADDEQ_DOUBLE)

    .def("__iadd__", [](Slice& s,const Trajectory& o) { return s += o;}, 
      SLICE_SLICE_OPERATORADDEQ_TRAJECTORY)

    .def("__iadd__", [](Slice& s,const Slice& o) { return s += o;}, 
      SLICE_SLICE_OPERATORADDEQ_SLICE)

    .def("__isub__", [](Slice& s,double o) { return s -= o;}, 
      SLICE_SLICE_OPERATORMINEQ_DOUBLE)

    .def("__isub__", [](Slice& s,const Trajectory& o) { return s -= o;}, 
      SLICE_SLICE_OPERATORMINEQ_TRAJECTORY)

    .def("__isub__", [](Slice& s,const Slice& o) { return s -= o;}, 
      SLICE_SLICE_OPERATORMINEQ_SLICE)

    .def("__imul__", [](Slice& s,double o) { return s *= o;}, 
      SLICE_SLICE_OPERATORMULEQ_DOUBLE)

    .def("__imul__", [](Slice& s,const Trajectory& o) { return s *= o;}, 
      SLICE_SLICE_OPERATORMULEQ_TRAJECTORY)

    .def("__imul__", [](Slice& s,const Slice& o) { return s *= o;}, 
      SLICE_SLICE_OPERATORMULEQ_SLICE)

    .def("__itruediv__", [](Slice& s,double o) { return s /= o;}, 
      SLICE_SLICE_OPERATORDIVEQ_DOUBLE)

    .def("__itruediv__", [](Slice& s,const Trajectory& o) { return s /= o;}, 
      SLICE_SLICE_OPERATORDIVEQ_TRAJECTORY)

    .def("__itruediv__", [](Slice& s,const Slice& o) { return s /= o;}, 
      SLICE_SLICE_OPERATORDIVEQ_SLICE)

    .def("__ior__", [](Slice& s,double o) { return s |= o;}, 
      SLICE_SLICE_OPERATORUNIEQ_DOUBLE)

    .def("__ior__", [](Slice& s,const Trajectory& o) { return s |= o;}, 
      SLICE_SLICE_OPERATORUNIEQ_TRAJECTORY)

    .def("__ior__", [](Slice& s,const Slice& o) { return s |= o;}, 
      SLICE_SLICE_OPERATORUNIEQ_SLICE)

    .def("__iand__", [](Slice& s,double o) { return s &= o;}, 
      SLICE_SLICE_OPERATORINTEQ_DOUBLE)

    .def("__iand__", [](Slice& s,const Trajectory& o) { return s &= o;}, 
      SLICE_SLICE_OPERATORINTEQ_TRAJECTORY)

    .def("__iand__", [](Slice& s,const Slice& o) { return s &= o;}, 
      SLICE_SLICE_OPERATORINTEQ_SLICE)
  
  // String

    .def("class_name", &Slice::class_name,
      SLICE_CONSTSTRING_CLASS_NAME)

    .def("__repr__", [](const Slice& x) { ostringstream str; str << x; return str.str(); })
  ;
}