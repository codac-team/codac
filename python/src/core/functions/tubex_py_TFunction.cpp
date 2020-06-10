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

#include "tubex_TFnc.h"
#include "tubex_TFunction.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

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


// class Tube;
// class TubeVector;


typedef const char* cc_ptr;
void export_TFunction(py::module& m, py::class_<TFnc>& fnc){

  //  py::class_<RandTrajectory> randtrajectory(m, "RandTrajectory", trajectory, "todo");
  py::class_<TFunction,TFnc> function(m, "TFunction", "todo");
  function
      .def(py::init<cc_ptr>(),"todo", "y"_a)
      .def(py::init<cc_ptr,cc_ptr>(),
          "todo", "x1"_a, "y"_a)
      .def(py::init<cc_ptr,cc_ptr,cc_ptr>(),
          "todo", "x1"_a, "x2"_a, "y"_a)
      .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
          "todo", "x1"_a, "x2"_a, "x3"_a, "y"_a)
      .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
          "todo", "x1"_a, "x2"_a, "x3"_a, "x4"_a, "y"_a)
      .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
          "todo", "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "y"_a)
      .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
          "todo", "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "y"_a)
      .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
          "todo", "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "y"_a)
      .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
          "todo", "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "x8"_a, "y"_a)
      // .def(py::init<int,cc_ptr *,cc_ptr>(),
          // DOCS_FUNCTION_FUNCTION_INT_CHAR_CHAR, "n"_a, "x"_a, "y"_a)
      .def(py::init<const tubex::TFunction &>(),
          "todo", "f"_a)

      .def("eval", (const Tube (TFunction::*)(const TubeVector &) const)&TFunction::eval,
          "todo", "x"_a)
      .def("eval", (const ibex::Interval (TFunction::*)(const ibex::Interval &) const)&TFunction::eval,
          "todo", "t"_a)
      .def("eval", (const ibex::Interval (TFunction::*)(const ibex::IntervalVector &) const)&TFunction::eval,
          "todo", "x"_a)
      .def("eval", (const ibex::Interval (TFunction::*)(int,const TubeVector &) const)&TFunction::eval,
          "todo", "slice_id"_a, "x"_a)
      .def("eval", (const ibex::Interval (TFunction::*)(const ibex::Interval &,const TubeVector &) const)&TFunction::eval,
          "todo", "t"_a, "x"_a)
      .def("traj_eval", &TFunction::traj_eval,
          "todo", "x"_a)
      .def("eval_vector", (const TubeVector (TFunction::*)(const TubeVector &) const)&TFunction::eval_vector,
          "todo", "x"_a)
      .def("eval_vector", (const ibex::IntervalVector (TFunction::*)(const ibex::Interval &) const)&TFunction::eval_vector,
          "todo", "t"_a)
      .def("eval_vector", (const ibex::IntervalVector (TFunction::*)(const ibex::IntervalVector &) const)&TFunction::eval_vector,
          "todo", "x"_a)
      .def("eval_vector", (const ibex::IntervalVector (TFunction::*)(int,const TubeVector &) const)&TFunction::eval_vector,
          "todo", "slice_id"_a, "x"_a)
      .def("eval_vector", (const ibex::IntervalVector (TFunction::*)(const ibex::Interval &,const TubeVector &) const)&TFunction::eval_vector,
          "todo", "t"_a, "x"_a)
      .def("traj_eval_vector", &TFunction::traj_eval_vector,
          "todo", "x"_a)
      .def("diff", &TFunction::diff,"todo")

      .def("__getitem__", [](TFunction& s, size_t index){
              if ((int)index >= s.nb_vars()){
                  throw py::index_error();
              }
                return s[static_cast<int>(index)];
          }, "todo", py::return_value_policy::reference_internal)
    ;
}
