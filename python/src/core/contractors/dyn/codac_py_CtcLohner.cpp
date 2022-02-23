/** 
 *  \file
 *  CtcLohner Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Auguste Bourgois, Simon Rohou
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
#include "codac_CtcLohner.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcLohner_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcLohner(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc)
{
  py::class_<CtcLohner> ctc_picard(m, "CtcLohner", dyn_ctc, CTCLOHNER_MAIN);
  ctc_picard

    .def(py::init<const Function&,int,double>(),
      CTCLOHNER_CTCLOHNER_FUNCTION_INT_DOUBLE,
      "f"_a, "contractions"_a=5, "eps"_a=0.1)

    .def("contract", (void (CtcLohner::*)(TubeVector&,TimePropag) )&CtcLohner::contract,
      CTCLOHNER_VOID_CONTRACT_TUBEVECTOR_TIMEPROPAG,
      "x"_a.noconvert(), "t_propa"_a=TimePropag::FORWARD|TimePropag::BACKWARD)
  ;
}