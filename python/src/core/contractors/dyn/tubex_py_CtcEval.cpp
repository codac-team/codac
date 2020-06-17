/** 
 *  \file
 *  CtcEval Python binding
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

#include "tubex_py_DynCtc.h"
#include "tubex_CtcEval.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcEval_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcEval(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc)
{
  py::class_<CtcEval> ctc_eval(m, "CtcEval", dyn_ctc, CTCEVAL_MAIN);
  ctc_eval

    .def(py::init<>(),
      CTCEVAL_CTCEVAL)

    .def("enable_time_propag", &CtcEval::enable_time_propag,
      CTCEVAL_VOID_ENABLE_TIME_PROPAG_BOOL,
      "enable_propagation"_a)

    .def("contract", (void (CtcEval::*)(double,Interval&,Tube&,Tube&))&CtcEval::contract,
      CTCEVAL_VOID_CONTRACT_DOUBLE_INTERVAL_TUBE_TUBE,
      "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())

    .def("contract", (void (CtcEval::*)(Interval&,Interval&,Tube&,Tube&))&CtcEval::contract,
      CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE,
      "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())

    .def("contract", (void (CtcEval::*)(double,IntervalVector&,TubeVector&,TubeVector&))&CtcEval::contract,
      CTCEVAL_VOID_CONTRACT_DOUBLE_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR,
      "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    
    .def("contract", (void (CtcEval::*)(Interval&,IntervalVector&,TubeVector&,TubeVector&))&CtcEval::contract,
      CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR,
      "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    
    .def("contract", (void (CtcEval::*)(Interval &,Interval &,const Tube&))&CtcEval::contract,
      CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVAL_TUBE,
      "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert())
    
    .def("contract", (void (CtcEval::*)(Interval &,IntervalVector &,const TubeVector&))&CtcEval::contract,
      CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR,
      "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert())
  ;
}