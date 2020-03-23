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

#include "tubex_Fnc.h"
#include "tubex_Function.h"
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




class pyFnc : public Fnc {
  // protected:
public:
  // pyFnc(int v): Fnc(v){}
  /* Inherit the constructors */
  using Fnc::Fnc;

  /* Trampoline (need one for each virtual function) */
  // void contract(IntervalVector& box) override {
  //   // py::gil_scoped_acquire acquire;
  //   PYBIND11_OVERLOAD_PURE(
  //     void,       /* return type */
  //     Fnc,        /* Parent class */
  //     contract,   /* Name of function */
  //     box         /* Argument(s) */
  //   );
  //   // py::gil_scoped_release release;
  // }
};

typedef const char* cc_ptr;
void export_Function(py::module& m){

  //  py::class_<Fnc, pyFnc> fnc(m, "Fnc");
  //   fnc
  //     .def(py::init<int,int,bool>(), "n"_a, "m"_a, "is_intertemporal"_a)
  //     .def("nb_vars", &Fnc::nb_vars)
  //     .def("image_dim", &Fnc::image_dim)
  //     .def("is_intertemporal", &Fnc::is_intertemporal)
  //     .def("eval", (const ibex::Interval (Fnc::*)(int, const tubex::TubeVector&) const)&Fnc::eval, "slice_id"_a, "x"_a)
  //     .def("eval", (const Tube (Fnc::*)( const tubex::TubeVector&) const)&Fnc::eval, "x"_a)
  //     .def("eval", (const ibex::Interval (Fnc::*)( const ibex::IntervalVector&) const)&Fnc::eval, "x"_a)
  //     .def("eval", (const ibex::Interval (Fnc::*)( const ibex::Interval&, const tubex::TubeVector&) const)&Fnc::eval, "t"_a, "x"_a)
  //     .def("eval_vector", (const TubeVector (Fnc::*)( const tubex::TubeVector&) const)&Fnc::eval_vector, "x"_a)
  //     .def("eval_vector", (const ibex::IntervalVector (Fnc::*)( const ibex::IntervalVector&) const)&Fnc::eval_vector, "x"_a)
  //     .def("eval_vector", (const ibex::IntervalVector (Fnc::*)(int, const tubex::TubeVector&) const)&Fnc::eval_vector, "slice_id"_a, "x"_a)
  //     .def("eval_vector", (const ibex::IntervalVector (Fnc::*)( const ibex::Interval&, const tubex::TubeVector&) const)&Fnc::eval_vector, "t"_a, "x"_a)
  //   ;


   py::class_<Function> function(m, "Function");
    function
      .def(py::init< cc_ptr>(), "y"_a)
      .def(py::init< cc_ptr, cc_ptr>(), "x1"_a, "y"_a)
      .def(py::init< cc_ptr, cc_ptr, cc_ptr>(), "x1"_a, "x2"_a, "y"_a)
      .def(py::init< cc_ptr, cc_ptr, cc_ptr, cc_ptr>(), "x1"_a, "x2"_a, "x3"_a, "y"_a)
      .def(py::init< cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>(), "x1"_a, "x2"_a, "x3"_a, "x4"_a, "y"_a)
      .def(py::init< cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>(), "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "y"_a)
      .def(py::init< cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>(), "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "y"_a)
      .def(py::init< cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>(), "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "y"_a)
      .def(py::init< cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>(), "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "x8"_a, "y"_a)
      // .def(py::init<int, const char**, const char*>(), "n"_a, "x"_a, "y"_a)

      .def(py::init<tubex::Function>(), "f"_a)

      .def("ibex_function", &Function::ibex_function)
      .def("eval", (const Tube (Function::*)( const TubeVector&) const)&Function::eval, "x"_a)
      .def("eval", (const ibex::Interval (Function::*)( const ibex::Interval&) const)&Function::eval, "t"_a)
      .def("eval", (const ibex::Interval (Function::*)( const ibex::IntervalVector&) const)&Function::eval, "x"_a)
      .def("eval", (const ibex::Interval (Function::*)(int, const TubeVector&) const)&Function::eval, "slice_id"_a, "x"_a)
      .def("eval", (const ibex::Interval (Function::*)( const ibex::Interval&, const TubeVector&) const)&Function::eval, "t"_a, "x"_a)
      .def("traj_eval", &Function::traj_eval, "x"_a)
      .def("eval_vector", (const TubeVector (Function::*)( const TubeVector&) const)&Function::eval_vector, "x"_a)
      .def("eval_vector", (const ibex::IntervalVector (Function::*)( const ibex::Interval&) const)&Function::eval_vector, "t"_a)
      .def("eval_vector", (const ibex::IntervalVector (Function::*)( const ibex::IntervalVector&) const)&Function::eval_vector, "x"_a)
      .def("eval_vector", (const ibex::IntervalVector (Function::*)(int, const TubeVector&) const)&Function::eval_vector, "slice_id"_a, "x"_a)
      .def("eval_vector", (const ibex::IntervalVector (Function::*)( const ibex::Interval&, const TubeVector&) const)&Function::eval_vector, "t"_a, "x"_a)
      .def("traj_eval_vector", &Function::traj_eval_vector, "x"_a)
      .def("diff", &Function::diff)
    ;
}
