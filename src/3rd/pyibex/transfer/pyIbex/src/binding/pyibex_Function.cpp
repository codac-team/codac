//============================================================================
//                               P Y I B E X
// File        : pyIbex_Function.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include "ibex_Function.h"
#include "ibex_SyntaxError.h"

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <sstream>

namespace py = pybind11;

using py::self;
using namespace ibex;

typedef const char* cc_ptr;
typedef void (Function::*backward_1) (const IntervalVector&, IntervalVector&) const;
std::string to_string(const Function& f){
  std::stringstream ss;
  ss << f;
  return ss.str();
}

// typedef void (Function::*backward_1) (const IntervalVector&, IntervalVector&) const;

void export_Function(py::module& m){

  py::register_exception_translator([](std::exception_ptr p) {
    try {
        if (p) std::rethrow_exception(p);
    } catch (const ibex::SyntaxError &e) {
      std::stringstream ss;
      ss << "\n############# IBEX FUNCTION PARSER ###############\n";
      ss << e;
      PyErr_SetString(PyExc_RuntimeError, ss.str().c_str());
    }
  });


  py::class_<Function>(m, "Function")

    .def(py::init<cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def("__repr__", &to_string)
    .def( "eval" , ( Interval(Function::*) (const IntervalVector& box) const) &Function::eval)
    .def( "eval_vector" , ( IntervalVector(Function::*) (const IntervalVector& box) const) &Function::eval_vector)
    // .def( "eval_matrix" , &Function::eval_matrix)
    .def( "eval", [](Function& f, Interval& itv) { return f.eval_vector(IntervalVector(1, itv));})

    .def( "backward", ( bool(Function::*) (const Interval& , IntervalVector&) const) &Function::backward)
    .def( "backward", ( bool(Function::*) (const IntervalVector& , IntervalVector&) const) &Function::backward)
    .def( "backward", ( bool(Function::*) (const IntervalMatrix& , IntervalVector&) const) &Function::backward)

    .def( "nb_arg", &Function::nb_arg)
    .def( "diff", &Function::diff)
    // .def( "gradient" , &Function::gradient)
    // .def( "jacobian" , &Function::jacobian)
    ;
}
