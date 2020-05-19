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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "tubex_Slice.h"
#include "tubex_py_Slice_docs.h"

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;

void export_Slice(py::module& m){

  py::class_<Slice> slice(m, "Slice");
  slice
    .def(py::init<const ibex::Interval &,const ibex::Interval &>(),
        DOCS_SLICE_SLICE_INTERVAL_INTERVAL, "domain"_a, "codomain"_a=ibex::Interval::ALL_REALS)
    .def(py::init<const Slice &>(),DOCS_SLICE_SLICE_SLICE, "x"_a)
    .def("size", &Slice::size,DOCS_SLICE_SIZE)
  
    .def("tdomain", &Slice::tdomain,DOCS_SLICE_TDOMAIN)
    .def("prev_slice", (Slice * (Slice::*)() )&Slice::prev_slice,
        DOCS_SLICE_PREV_SLICE)
    .def("prev_slice", (const Slice * (Slice::*)() const)&Slice::prev_slice,
        DOCS_SLICE_PREV_SLICE1)
    .def("next_slice", (Slice * (Slice::*)() )&Slice::next_slice,
        DOCS_SLICE_NEXT_SLICE)
    .def("next_slice", (const Slice * (Slice::*)() const)&Slice::next_slice,
        DOCS_SLICE_NEXT_SLICE1)
    .def("input_gate", &Slice::input_gate,DOCS_SLICE_INPUT_GATE)
    .def("output_gate", &Slice::output_gate,DOCS_SLICE_OUTPUT_GATE)
    .def("polygon", &Slice::polygon,DOCS_SLICE_POLYGON_SLICE, "v"_a)
    .def("codomain", &Slice::codomain,DOCS_SLICE_CODOMAIN)
    .def("box", &Slice::box,DOCS_SLICE_BOX)
    // .def_static("diam", (double (Slice::*)(const ibex::Interval &) )&Slice::diam,
        // DOCS_SLICE_DIAM_INTERVAL, "interval"_a)
    .def("diam", (double (Slice::*)() const)&Slice::diam,DOCS_SLICE_DIAM)
    .def("volume", &Slice::volume,DOCS_SLICE_VOLUME)
    .def("__call__", [](Slice& s,double o) { return s(o);}, 
        DOCS_SLICE_OPERATOR_CALL_DOUBLE)
    .def("__call__", [](Slice& s,const ibex::Interval & o) { return s(o);}, 
        DOCS_SLICE_OPERATOR_CALL_INTERVAL)
    .def("eval", &Slice::eval,
        DOCS_SLICE_EVAL_INTERVAL, "t"_a=ibex::Interval::ALL_REALS)
    .def("interpol", (const ibex::Interval (Slice::*)(double,const Slice &) const)&Slice::interpol,
        DOCS_SLICE_INTERPOL_DOUBLE_SLICE, "t"_a, "v"_a)
    .def("interpol", (const ibex::Interval (Slice::*)(const ibex::Interval &,const Slice &) const)&Slice::interpol,
        DOCS_SLICE_INTERPOL_INTERVAL_SLICE, "t"_a, "v"_a)
    .def("invert", (const ibex::Interval (Slice::*)(const ibex::Interval &,const ibex::Interval &) const)&Slice::invert,
        DOCS_SLICE_INVERT_INTERVAL_INTERVAL, "y"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
    .def("invert", (const ibex::Interval (Slice::*)(const ibex::Interval &,const Slice &,const ibex::Interval &) const)&Slice::invert,
        DOCS_SLICE_INVERT_INTERVAL_SLICE_INTERVAL, "y"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
    .def("__eq__", [](Slice& s,const Slice & o) { return s == o;}, 
        DOCS_SLICE_EQ_SLICE)
    .def("__ne__", [](Slice& s,const Slice & o) { return s != o;}, 
        DOCS_SLICE_NE_SLICE)
    .def("is_subset", &Slice::is_subset,DOCS_SLICE_IS_SUBSET_SLICE, "x"_a)
    .def("is_strict_subset", &Slice::is_strict_subset,
        DOCS_SLICE_IS_STRICT_SUBSET_SLICE, "x"_a)
    .def("is_interior_subset", &Slice::is_interior_subset,
        DOCS_SLICE_IS_INTERIOR_SUBSET_SLICE, "x"_a)
    .def("is_strict_interior_subset", &Slice::is_strict_interior_subset,
        DOCS_SLICE_IS_STRICT_INTERIOR_SUBSET_SLICE, "x"_a)
    .def("is_superset", &Slice::is_superset,DOCS_SLICE_IS_SUPERSET_SLICE, "x"_a)
    .def("is_strict_superset", &Slice::is_strict_superset,
        DOCS_SLICE_IS_STRICT_SUPERSET_SLICE, "x"_a)
    .def("is_empty", &Slice::is_empty,DOCS_SLICE_IS_EMPTY)
    .def("contains", &Slice::contains,DOCS_SLICE_CONTAINS_TRAJECTORY, "x"_a)
    .def("set", &Slice::set,DOCS_SLICE_SET_INTERVAL, "y"_a)
    .def("set_empty", &Slice::set_empty,DOCS_SLICE_SET_EMPTY)
    .def("set_envelope", &Slice::set_envelope,
        DOCS_SLICE_SET_ENVELOPE_INTERVAL_BOOL, "envelope"_a, "slice_consistency"_a=true)
    .def("set_input_gate", &Slice::set_input_gate,
        DOCS_SLICE_SET_INPUT_GATE_INTERVAL_BOOL, "input_gate"_a, "slice_consistency"_a=true)
    .def("set_output_gate", &Slice::set_output_gate,
        DOCS_SLICE_SET_OUTPUT_GATE_INTERVAL_BOOL, "output_gate"_a, "slice_consistency"_a=true)
    .def("inflate", &Slice::inflate,DOCS_SLICE_INFLATE_DOUBLE, "rad"_a)
    .def("__iadd__", [](Slice& s,double o) { return s += o;}, 
        DOCS_SLICE_IADD_DOUBLE)
    .def("__iadd__", [](Slice& s,const Trajectory & o) { return s += o;}, 
        DOCS_SLICE_IADD_TRAJECTORY)
    .def("__iadd__", [](Slice& s,const Slice & o) { return s += o;}, 
        DOCS_SLICE_IADD_SLICE)
    .def("__isub__", [](Slice& s,double o) { return s -= o;}, 
        DOCS_SLICE_ISUB_DOUBLE)
    .def("__isub__", [](Slice& s,const Trajectory & o) { return s -= o;}, 
        DOCS_SLICE_ISUB_TRAJECTORY)
    .def("__isub__", [](Slice& s,const Slice & o) { return s -= o;}, 
        DOCS_SLICE_ISUB_SLICE)
    .def("__imul__", [](Slice& s,double o) { return s *= o;}, 
        DOCS_SLICE_IMUL_DOUBLE)
    .def("__imul__", [](Slice& s,const Trajectory & o) { return s *= o;}, 
        DOCS_SLICE_IMUL_TRAJECTORY)
    .def("__imul__", [](Slice& s,const Slice & o) { return s *= o;}, 
        DOCS_SLICE_IMUL_SLICE)
    .def("__itruediv__", [](Slice& s,double o) { return s /= o;}, 
        DOCS_SLICE_ITRUEDIV_DOUBLE)
    .def("__itruediv__", [](Slice& s,const Trajectory & o) { return s /= o;}, 
        DOCS_SLICE_ITRUEDIV_TRAJECTORY)
    .def("__itruediv__", [](Slice& s,const Slice & o) { return s /= o;}, 
        DOCS_SLICE_ITRUEDIV_SLICE)
    .def("__ior__", [](Slice& s,double o) { return s |= o;}, 
        DOCS_SLICE_IOR_DOUBLE)
    .def("__ior__", [](Slice& s,const Trajectory & o) { return s |= o;}, 
        DOCS_SLICE_IOR_TRAJECTORY)
    .def("__ior__", [](Slice& s,const Slice & o) { return s |= o;}, 
        DOCS_SLICE_IOR_SLICE)
    .def("__iand__", [](Slice& s,double o) { return s &= o;}, 
        DOCS_SLICE_IAND_DOUBLE)
    .def("__iand__", [](Slice& s,const Trajectory & o) { return s &= o;}, 
        DOCS_SLICE_IAND_TRAJECTORY)
    .def("__iand__", [](Slice& s,const Slice & o) { return s &= o;}, 
        DOCS_SLICE_IAND_SLICE)
    .def("class_name", &Slice::class_name,DOCS_SLICE_CLASS_NAME)

      // .def(py::init<ibex::Interval,ibex::Interval>(), "domain"_a, "codomain"_a=ibex::Interval::ALL_REALS)
      // .def(py::init<tubex::Slice>(), "x"_a)
      // .def("size", &Slice::size)
      // .def("domain", &Slice::domain)
      // .def("prev_slice", (Slice * (Slice::*)() )&Slice::prev_slice)
      // .def("prev_slice", (const Slice * (Slice::*)() const)&Slice::prev_slice)
      // .def("next_slice", (Slice * (Slice::*)() )&Slice::next_slice)
      // .def("next_slice", (const Slice * (Slice::*)() const)&Slice::next_slice)
      // .def("input_gate", &Slice::input_gate)
      // .def("output_gate", &Slice::output_gate)
      // .def("polygon", &Slice::polygon, "v"_a)
      // .def("codomain", &Slice::codomain)
      // .def("box", &Slice::box)
      // // .def("diam", (static double (Slice::*)( const ibex::Interval&) )&Slice::diam, "interval"_a)
      // .def("diam", (double (Slice::*)() const)&Slice::diam)
      // .def("volume", &Slice::volume)
      // .def("eval", &Slice::eval, "t"_a=ibex::Interval::ALL_REALS)
      // .def("interpol", (const ibex::Interval (Slice::*)(double, const tubex::Slice&) const)&Slice::interpol, "t"_a, "v"_a)
      // .def("interpol", (const ibex::Interval (Slice::*)( const ibex::Interval&, const tubex::Slice&) const)&Slice::interpol, "t"_a, "v"_a)
      // .def("invert", (const ibex::Interval (Slice::*)( const ibex::Interval&, const ibex::Interval&) const)&Slice::invert, "y"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      // .def("invert", (const ibex::Interval (Slice::*)( const ibex::Interval&, const tubex::Slice&, const ibex::Interval&) const)&Slice::invert, "y"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      // .def("is_subset", &Slice::is_subset, "x"_a)
      // .def("is_strict_subset", &Slice::is_strict_subset, "x"_a)
      // .def("is_interior_subset", &Slice::is_interior_subset, "x"_a)
      // .def("is_strict_interior_subset", &Slice::is_strict_interior_subset, "x"_a)
      // .def("is_superset", &Slice::is_superset, "x"_a)
      // .def("is_strict_superset", &Slice::is_strict_superset, "x"_a)
      // .def("is_empty", &Slice::is_empty)
      // .def("contains", &Slice::contains, "x"_a)
      // .def("set", &Slice::set, "y"_a)
      // .def("set_empty", &Slice::set_empty)
      // .def("set_envelope", &Slice::set_envelope, "envelope"_a, "slice_consistency"_a=true)
      // .def("set_input_gate", &Slice::set_input_gate, "input_gate"_a, "slice_consistency"_a=true)
      // .def("set_output_gate", &Slice::set_output_gate, "output_gate"_a, "slice_consistency"_a=true)
      // .def("inflate", &Slice::inflate, "rad"_a)
      // .def("class_name", &Slice::class_name);
  ;

}