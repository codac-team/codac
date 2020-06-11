/** 
 *  \file
 *  CtcPicard Python binding
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
#include "tubex_CtcPicard.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcPicard_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcPicard(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc)
{
  py::class_<CtcPicard> ctc_picard(m, "CtcPicard", dyn_ctc, "todo");
  ctc_picard

    .def(py::init<float>(),
      CTCPICARD_CTCPICARD_FLOAT,
      "delta"_a=1.1)

    .def("contract", (void (CtcPicard::*)(const TFnc&,Tube&,TimePropag))&CtcPicard::contract,
      CTCPICARD_VOID_CONTRACT_TFNC_TUBE_TIMEPROPAG,
      "f"_a, "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)

    .def("contract", (void (CtcPicard::*)(const TFnc&,TubeVector&,TimePropag) )&CtcPicard::contract,
      CTCPICARD_VOID_CONTRACT_TFNC_TUBEVECTOR_TIMEPROPAG,
      "f"_a, "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)

    .def("picard_iterations", &CtcPicard::picard_iterations,
      CTCPICARD_INT_PICARD_ITERATIONS)
  ;
}