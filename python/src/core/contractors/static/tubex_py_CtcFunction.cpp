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
#include "tubex_py_CtcFunction_docs.h"


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


void export_CtcFunction(py::module& m, py::class_<Ctc, pyCtc>& static_ctc){

    //py::class_<Ctc, pyStaticCtc> static_ctc(m, "StaticCtc", py::module_local());

    py::class_<CtcFunction> ctc_function(m, "CtcFunction", static_ctc, "todo");
    ctc_function
      .def(py::init<const ibex::Function &>(),
          CTCFUNCTION_CTCFUNCTION_FUNCTION, "f"_a)
      .def(py::init<const ibex::Function &,const ibex::Domain &>(),
          CTCFUNCTION_CTCFUNCTION_FUNCTION_DOMAIN, "f"_a, "y"_a)
      .def(py::init<const ibex::Function &,const ibex::Interval &>(),
          CTCFUNCTION_CTCFUNCTION_FUNCTION_INTERVAL, "f"_a, "y"_a)
      .def(py::init<const ibex::Function &,const ibex::IntervalVector &>(),
          CTCFUNCTION_CTCFUNCTION_FUNCTION_INTERVALVECTOR, "f"_a, "y"_a)
      .def("contract", (void (CtcFunction::*)(ibex::IntervalVector &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_INTERVALVECTOR, "x"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(TubeVector &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBEVECTOR, "x"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE, "x1"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &,Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &,Tube &,Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert(), "x5"_a.noconvert())
      .def("contract", (void (CtcFunction::*)(Tube &,Tube &,Tube &,Tube &,Tube &,Tube &) )&CtcFunction::contract,
          CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE_TUBE_TUBE, "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert(), "x5"_a.noconvert(), "x6"_a.noconvert())
      //.def("contract", (void (CtcFunction::*)(Slice * *) )&CtcFunction::contract,
      //    CTCFUNCTION_VOID_CONTRACT_SLICE, "v_x_slices"_a)
    ;
}