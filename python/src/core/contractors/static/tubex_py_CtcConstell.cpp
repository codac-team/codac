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

#include "tubex_py_Ctc.h"

// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcConstell_docs.h"


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


void export_CtcConstell(py::module& m, py::class_<Ctc, pyCtc>& static_ctc){
    py::class_<CtcConstell> ctc_constell(m, "CtcConstell", static_ctc, "todo");
    ctc_constell
      .def(py::init<const std::list<ibex::IntervalVector> &>(),
          CTCCONSTELL_CTCCONSTELL_VECTORINTERVALVECTOR, "map"_a)
      .def("contract", &CtcConstell::contract,
          CTCCONSTELL_VOID_CONTRACT_INTERVALVECTOR, "beacon_box"_a.noconvert())
    ;
}