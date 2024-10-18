/** 
 *  \file
 *  CtcPicard Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_py_DynCtc.h"
#include "codac_CtcPicard.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcPicard_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcPicard(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc)
{
  py::class_<CtcPicard> ctc_picard(m, "CtcPicard", dyn_ctc, CTCPICARD_MAIN);
  ctc_picard

    .def(py::init<Function&,float>(),
      CTCPICARD_CTCPICARD_FUNCTION_FLOAT,
      "f"_a, "delta"_a=1.1)

    .def(py::init<TFnc&,float>(),
      CTCPICARD_CTCPICARD_TFNC_FLOAT,
      "f"_a, "delta"_a=1.1)

    .def("contract", (void (CtcPicard::*)(Tube&,TimePropag))&CtcPicard::contract,
      CTCPICARD_VOID_CONTRACT_TUBE_TIMEPROPAG,
      "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)

    .def("contract", (void (CtcPicard::*)(TubeVector&,TimePropag) )&CtcPicard::contract,
      CTCPICARD_VOID_CONTRACT_TUBEVECTOR_TIMEPROPAG,
      "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)

    .def("picard_iterations", &CtcPicard::picard_iterations,
      CTCPICARD_INT_PICARD_ITERATIONS)
  ;
}