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
#include "tubex_py_CtcPicard_docs.h"


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


void export_CtcPicard(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc){

    py::class_<CtcPicard> ctc_picard(m, "CtcPicard", dyn_ctc, "todo");
    ctc_picard
      .def(py::init<float>(),CTCPICARD_CTCPICARD_FLOAT, "delta"_a=1.1)
      //.def("contract", (void (CtcPicard::*)(std::vector<Domain *> &) )&CtcPicard::contract,
      //    DOCS_CTCPICARD_CONTRACT_VECTOR_DOMAIN, "v_domains"_a)
      .def("contract", (void (CtcPicard::*)(const TFnc &,Tube &,TimePropag) )&CtcPicard::contract,
          CTCPICARD_VOID_CONTRACT_TFNC_TUBE_TIMEPROPAG, "f"_a, "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
      .def("contract", (void (CtcPicard::*)(const TFnc &,TubeVector &,TimePropag) )&CtcPicard::contract,
          CTCPICARD_VOID_CONTRACT_TFNC_TUBEVECTOR_TIMEPROPAG, "f"_a, "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
      .def("picard_iterations", &CtcPicard::picard_iterations,
          CTCPICARD_INT_PICARD_ITERATIONS)
    ;
}