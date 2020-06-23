/** 
 *  \file
 *  TFunction Python binding
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
#include "pyIbex_type_caster.h"

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"
#include "tubex_TFunction.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_TFunction_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


typedef const char* cc_ptr;

void export_TFunction(py::module& m, py::class_<TFnc>& fnc)
{
  py::class_<TFunction,TFnc> function(m, "TFunction", TFUNCTION_MAIN);
  function

    .def(py::init<cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR,
      "y"_a)

    .def(py::init<cc_ptr,cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR_CHAR,
      "x1"_a, "y"_a)

    .def(py::init<cc_ptr,cc_ptr,cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR_CHAR_CHAR,
      "x1"_a, "x2"_a, "y"_a)

    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR_CHAR_CHAR_CHAR,
      "x1"_a, "x2"_a, "x3"_a, "y"_a)

    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR_CHAR_CHAR_CHAR_CHAR,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a, "y"_a)

    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "y"_a)

    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "y"_a)

    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "y"_a)

    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>(),
      TFUNCTION_TFUNCTION_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR,
      "x1"_a, "x2"_a, "x3"_a, "x4"_a, "x5"_a, "x6"_a, "x7"_a, "x8"_a, "y"_a)

    //.def(py::init<int,const char**,const char*>(),
    //  TFUNCTION_TFUNCTION_INT_CHAR_CHAR,
    //  "n"_a, "x"_a, "y"_a)

    .def(py::init<const TFunction&>(),
      TFUNCTION_TFUNCTION_TFUNCTION, 
      "f"_a)

    .def("expr", &TFunction::expr,
      TFUNCTION_CONSTSTRING_EXPR)

    .def("arg_name", &TFunction::arg_name,
      TFUNCTION_CONSTSTRING_ARG_NAME_INT,
      "i"_a)

    .def("eval", (const Tube (TFunction::*)(const TubeVector&) const)&TFunction::eval,
      TFUNCTION_CONSTTUBE_EVAL_TUBEVECTOR,
      "x"_a)

    .def("traj_eval", &TFunction::traj_eval,
      TFUNCTION_CONSTTRAJECTORY_TRAJ_EVAL_TRAJECTORYVECTOR,
      "x"_a)

    .def("eval", (const Interval (TFunction::*)(const Interval&) const)&TFunction::eval,
      TFUNCTION_CONSTINTERVAL_EVAL_INTERVAL,
      "t"_a)

    .def("eval", (const Interval (TFunction::*)(const IntervalVector&) const)&TFunction::eval,
      TFUNCTION_CONSTINTERVAL_EVAL_INTERVALVECTOR,
      "x"_a)

    .def("eval", (const Interval (TFunction::*)(int,const TubeVector&) const)&TFunction::eval,
      TFUNCTION_CONSTINTERVAL_EVAL_INT_TUBEVECTOR,
      "slice_id"_a, "x"_a)

    .def("eval", (const Interval (TFunction::*)(const Interval&,const TubeVector&) const)&TFunction::eval,
      TFUNCTION_CONSTINTERVAL_EVAL_INTERVAL_TUBEVECTOR,
      "t"_a, "x"_a)

    .def("eval_vector", (const TubeVector (TFunction::*)(const TubeVector&) const)&TFunction::eval_vector,
      TFUNCTION_CONSTTUBEVECTOR_EVAL_VECTOR_TUBEVECTOR,
      "x"_a)

    .def("traj_eval_vector", &TFunction::traj_eval_vector,
      TFUNCTION_CONSTTRAJECTORYVECTOR_TRAJ_EVAL_VECTOR_TRAJECTORYVECTOR,
      "x"_a)

    .def("eval_vector", (const IntervalVector (TFunction::*)(const Interval&) const)&TFunction::eval_vector,
      TFUNCTION_CONSTINTERVALVECTOR_EVAL_VECTOR_INTERVAL,
      "t"_a)

    .def("eval_vector", (const IntervalVector (TFunction::*)(const IntervalVector&) const)&TFunction::eval_vector,
      TFUNCTION_CONSTINTERVALVECTOR_EVAL_VECTOR_INTERVALVECTOR,
      "x"_a)

    .def("eval_vector", (const IntervalVector (TFunction::*)(int,const TubeVector&) const)&TFunction::eval_vector,
      TFUNCTION_CONSTINTERVALVECTOR_EVAL_VECTOR_INT_TUBEVECTOR,
      "slice_id"_a, "x"_a)

    .def("eval_vector", (const IntervalVector (TFunction::*)(const Interval&,const TubeVector&) const)&TFunction::eval_vector,
      TFUNCTION_CONSTINTERVALVECTOR_EVAL_VECTOR_INTERVAL_TUBEVECTOR,
      "t"_a, "x"_a)

    .def("diff", &TFunction::diff,
      TFUNCTION_CONSTTFUNCTION_DIFF)

  // Python vector methods

    .def("__getitem__", [](TFunction& s, size_t index)
      {
        if((int)index >= s.nb_vars())
          throw py::index_error();
              
        return s[static_cast<int>(index)];
      },
      TFUNCTION_CONSTTFUNCTION_OPERATORB_INT,
      py::return_value_policy::reference_internal)
  ;
}
