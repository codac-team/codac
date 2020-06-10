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

#include "tubex_DynCtc.h"
#include "tubex_CtcFunction.h"
#include "tubex_CtcDist.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_CtcPicard.h"
#include "tubex_CtcConstell.h"
#include "tubex_Domain.h"
#include "tubex_CtcFunction.h"

#include "tubex_py_DynCtc.h"

// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcEval_docs.h"


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
using ibex::Ctc;


void export_CtcEval(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc){

  py::class_<CtcEval> ctc_eval(m, "CtcEval", dyn_ctc);
  ctc_eval
    .def(py::init<>(),CTCEVAL_CTCEVAL)
    //.def("contract", (void (CtcEval::*)(std::vector<Domain *> &) )&CtcEval::contract,
    //    DOCS_CTCEVAL_CONTRACT_VECTOR_a.noconvert()BSTRACTDOMAIN, "v_domains"_a.noconvert())
    .def("contract", (void (CtcEval::*)(double,ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_DOUBLE_INTERVAL_TUBE_TUBE, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    //.def("contract", (void (CtcEval::*)(const ibex::Interval &,const ibex::Interval &,Tube &,Tube &) )&CtcEval::contract,
    //    CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    .def("contract", (void (CtcEval::*)(double,ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_DOUBLE_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    //.def("contract", (void (CtcEval::*)(const ibex::Interval &,const ibex::IntervalVector &,TubeVector &,TubeVector &) )&CtcEval::contract,
    //    DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR1, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert(), "w"_a.noconvert())
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::Interval &,const Tube &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVAL_TUBE, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert())
    .def("contract", (void (CtcEval::*)(ibex::Interval &,ibex::IntervalVector &,const TubeVector &) )&CtcEval::contract,
        CTCEVAL_VOID_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR, "t"_a.noconvert(), "z"_a.noconvert(), "y"_a.noconvert())
    .def("enable_time_propag", &CtcEval::enable_time_propag,
        CTCEVAL_VOID_ENABLE_TIME_PROPAG_BOOL, "enable_propagation"_a)
  ;

}