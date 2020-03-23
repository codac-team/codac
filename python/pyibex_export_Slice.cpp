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

#include "tubex_Slice.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

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
      .def(py::init<ibex::Interval,ibex::Interval>(), "domain"_a, "codomain"_a=ibex::Interval::ALL_REALS)
      .def(py::init<tubex::Slice>(), "x"_a)
      .def("size", &Slice::size)
      .def("domain", &Slice::domain)
      .def("prev_slice", (Slice * (Slice::*)() )&Slice::prev_slice)
      .def("prev_slice", (const Slice * (Slice::*)() const)&Slice::prev_slice)
      .def("next_slice", (Slice * (Slice::*)() )&Slice::next_slice)
      .def("next_slice", (const Slice * (Slice::*)() const)&Slice::next_slice)
      .def("input_gate", &Slice::input_gate)
      .def("output_gate", &Slice::output_gate)
      .def("polygon", &Slice::polygon, "v"_a)
      .def("codomain", &Slice::codomain)
      .def("box", &Slice::box)
      // .def("diam", (static double (Slice::*)( const ibex::Interval&) )&Slice::diam, "interval"_a)
      .def("diam", (double (Slice::*)() const)&Slice::diam)
      .def("volume", &Slice::volume)
      .def("eval", &Slice::eval, "t"_a=ibex::Interval::ALL_REALS)
      .def("interpol", (const ibex::Interval (Slice::*)(double, const tubex::Slice&) const)&Slice::interpol, "t"_a, "v"_a)
      .def("interpol", (const ibex::Interval (Slice::*)( const ibex::Interval&, const tubex::Slice&) const)&Slice::interpol, "t"_a, "v"_a)
      .def("invert", (const ibex::Interval (Slice::*)( const ibex::Interval&, const ibex::Interval&) const)&Slice::invert, "y"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("invert", (const ibex::Interval (Slice::*)( const ibex::Interval&, const tubex::Slice&, const ibex::Interval&) const)&Slice::invert, "y"_a, "v"_a, "search_domain"_a=ibex::Interval::ALL_REALS)
      .def("is_subset", &Slice::is_subset, "x"_a)
      .def("is_strict_subset", &Slice::is_strict_subset, "x"_a)
      .def("is_interior_subset", &Slice::is_interior_subset, "x"_a)
      .def("is_strict_interior_subset", &Slice::is_strict_interior_subset, "x"_a)
      .def("is_superset", &Slice::is_superset, "x"_a)
      .def("is_strict_superset", &Slice::is_strict_superset, "x"_a)
      .def("is_empty", &Slice::is_empty)
      .def("contains", &Slice::contains, "x"_a)
      .def("set", &Slice::set, "y"_a)
      .def("set_empty", &Slice::set_empty)
      .def("set_envelope", &Slice::set_envelope, "envelope"_a, "slice_consistency"_a=true)
      .def("set_input_gate", &Slice::set_input_gate, "input_gate"_a, "slice_consistency"_a=true)
      .def("set_output_gate", &Slice::set_output_gate, "output_gate"_a, "slice_consistency"_a=true)
      .def("inflate", &Slice::inflate, "rad"_a)
      .def("class_name", &Slice::class_name);
  ;

}