/** 
 *  \file
 *  CtcDeriv Python binding
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
#include "tubex_CtcDeriv.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcDeriv_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcDeriv(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc)
{
  py::class_<CtcDeriv> ctc_deriv(m, "CtcDeriv", dyn_ctc, CTCDERIV_MAIN);
  ctc_deriv

    .def(py::init<>(),
      CTCDERIV_CTCDERIV)

    .def("contract", (void (CtcDeriv::*)(Tube&,const Tube&,TimePropag))&CtcDeriv::contract,
      CTCDERIV_VOID_CONTRACT_TUBE_TUBE_TIMEPROPAG,
      "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)

    .def("contract", (void (CtcDeriv::*)(TubeVector&,const TubeVector&,TimePropag))&CtcDeriv::contract,
      CTCDERIV_VOID_CONTRACT_TUBEVECTOR_TUBEVECTOR_TIMEPROPAG,
      "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)

    .def("contract", (void (CtcDeriv::*)(Slice&,const Slice&,TimePropag))&CtcDeriv::contract,
      CTCDERIV_VOID_CONTRACT_SLICE_SLICE_TIMEPROPAG,
      "x"_a.noconvert(), "v"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
  ;
}