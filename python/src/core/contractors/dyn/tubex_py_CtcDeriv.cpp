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
#include "tubex_py_CtcDeriv_docs.h"


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

void export_CtcDeriv(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc){


    py::class_<CtcDeriv> ctc_deriv(m, "CtcDeriv", dyn_ctc, "todo");
    ctc_deriv
        .def(py::init<>(),CTCDERIV_CTCDERIV)
        // .def("contract", (void (CtcDeriv::*)(std::vector<Domain> &) )&CtcDeriv::contract,
            // DOCS_CTCDERIV_CONTRACT_VECTOR_ABSTRACTDOMAIN, "v_domains"_a)
        .def("contract", (void (CtcDeriv::*)(Tube &,const Tube &,TimePropag) )&CtcDeriv::contract,
            CTCDERIV_VOID_CONTRACT_TUBE_TUBE_TIMEPROPAG, "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
        .def("contract", (void (CtcDeriv::*)(TubeVector &,const TubeVector &,TimePropag) )&CtcDeriv::contract,
            CTCDERIV_VOID_CONTRACT_TUBEVECTOR_TUBEVECTOR_TIMEPROPAG, "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
        .def("contract", (void (CtcDeriv::*)(Slice &,const Slice &,TimePropag) )&CtcDeriv::contract,
            CTCDERIV_VOID_CONTRACT_SLICE_SLICE_TIMEPROPAG, "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
    ;

}