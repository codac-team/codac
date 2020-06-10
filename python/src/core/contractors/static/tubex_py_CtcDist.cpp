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
#include "tubex_py_CtcDist_docs.h"


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


void export_CtcDist(py::module& m, py::class_<Ctc, pyCtc>& static_ctc){

	//py::class_<Ctc, pyStaticCtc> static_ctc(m, "StaticCtc", py::module_local());

    py::class_<CtcDist> ctc_dist(m, "CtcDist", static_ctc, "todo");
    ctc_dist
      .def(py::init<>(),CTCDIST_CTCDIST)
      .def("contract", &CtcDist::contract,
          CTCDIST_VOID_CONTRACT_INTERVALVECTOR_INTERVALVECTOR_INTERVAL, "a"_a.noconvert(), "b"_a.noconvert(), "d"_a.noconvert())

    ;
}