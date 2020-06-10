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




class pyTFnc : public TFnc {
  // protected:
public:
  // pyTFnc(int v): TFnc(v){}
  /* Inherit the constructors */
  using TFnc::TFnc;
  const Tube eval(const TubeVector &x) const override {
    PYBIND11_OVERLOAD_PURE(const Tube, TFnc, eval, x);
  }
  const ibex::Interval eval(const ibex::IntervalVector &x) const override {
    PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, x);
  }
  const ibex::Interval eval(int slice_id,const TubeVector &x) const override {
    PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, slice_id, x);
  }
  const ibex::Interval eval(const ibex::Interval &t,const TubeVector &x) const override {
    PYBIND11_OVERLOAD_PURE(const ibex::Interval, TFnc, eval, t, x);
  }
  const TubeVector eval_vector(const TubeVector &x) const override {
    PYBIND11_OVERLOAD_PURE(const TubeVector, TFnc, eval_vector, x);
  }
  const ibex::IntervalVector eval_vector(const ibex::IntervalVector &x) const override {
    PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, x);
  }
  const ibex::IntervalVector eval_vector(int slice_id,const TubeVector &x) const override {
    PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, slice_id, x);
  }
  const ibex::IntervalVector eval_vector(const ibex::Interval &t,const TubeVector &x) const override {
    PYBIND11_OVERLOAD_PURE(const ibex::IntervalVector, TFnc, eval_vector, t, x);
  }
  /* Trampoline (need one for each virtual function) */
  // void contract(IntervalVector& box) override {
  //   // py::gil_scoped_acquire acquire;
  //   PYBIND11_OVERLOAD_PURE(
  //     void,       /* return type */
  //     TFnc,        /* Parent class */
  //     contract,   /* Name of function */
  //     box         /* Argument(s) */
  //   );
    // py::gil_scoped_release release;
  // }
};

typedef const char* cc_ptr;
py::class_<TFnc, pyTFnc> export_TFnc(py::module& m){

  py::class_<TFnc, pyTFnc> fnc(m, "TFnc");
  fnc
    .def(py::init<int,int,bool>(),
        "todo", "n"_a, "m"_a, "is_intertemporal"_a)
    .def("nb_vars", &TFnc::nb_vars, "todo")
    .def("image_dim", &TFnc::image_dim, "todo")
    .def("is_intertemporal", &TFnc::is_intertemporal, "todo")
    // .def("eval", (const Tube (TFnc::*)(const TubeVector &) const)&TFnc::eval,
    //     DOCS_FNC_EVAL_TUBEVECTOR, "x"_a)
    // .def("eval", (const ibex::Interval (TFnc::*)(const ibex::IntervalVector &) const)&TFnc::eval,
    //     DOCS_FNC_EVAL_INTERVALVECTOR, "x"_a)
    // .def("eval", (const ibex::Interval (TFnc::*)(int,const TubeVector &) const)&TFnc::eval,
    //     DOCS_FNC_EVAL_INT_TUBEVECTOR, "slice_id"_a, "x"_a)
    // .def("eval", (const ibex::Interval (TFnc::*)(const ibex::Interval &,const TubeVector &) const)&TFnc::eval,
    //     DOCS_FNC_EVAL_INTERVAL_TUBEVECTOR, "t"_a, "x"_a)
    // .def("eval_vector", (const TubeVector (TFnc::*)(const TubeVector &) const)&TFnc::eval_vector,
    //     DOCS_FNC_EVAL_VECTOR_TUBEVECTOR, "x"_a)
    // .def("eval_vector", (const ibex::IntervalVector (TFnc::*)(const ibex::IntervalVector &) const)&TFnc::eval_vector,
    //     DOCS_FNC_EVAL_VECTOR_INTERVALVECTOR, "x"_a)
    // .def("eval_vector", (const ibex::IntervalVector (TFnc::*)(int,const TubeVector &) const)&TFnc::eval_vector,
    //     DOCS_FNC_EVAL_VECTOR_INT_TUBEVECTOR, "slice_id"_a, "x"_a)
    // .def("eval_vector", (const ibex::IntervalVector (TFnc::*)(const ibex::Interval &,const TubeVector &) const)&TFnc::eval_vector,
    //     DOCS_FNC_EVAL_VECTOR_INTERVAL_TUBEVECTOR, "t"_a, "x"_a)
  ;

  return fnc;
}