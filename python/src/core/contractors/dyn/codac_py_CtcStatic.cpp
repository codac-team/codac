/** 
 *  \file
 *  CtcStatic Python binding
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "codac_py_DynCtc.h"
#include "codac_CtcStatic.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcStatic_docs.h"

using namespace std;
using namespace ibex;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcStatic(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc)
{
  py::class_<CtcStatic> ctc_static(m, "CtcStatic", dyn_ctc, CTCSTATIC_MAIN);
  ctc_static

    .def(py::init<Ctc&,bool>(),
      CTCSTATIC_CTCSTATIC_CTC_BOOL,
      "ibex_ctc"_a, "temporal_ctc"_a=false)

    .def("contract", (void (CtcStatic::*)(Tube&))&CtcStatic::contract,
      CTCSTATIC_VOID_CONTRACT_TUBE,
      "x"_a.noconvert())

    .def("contract", (void (CtcStatic::*)(TubeVector&) )&CtcStatic::contract,
      CTCSTATIC_VOID_CONTRACT_TUBEVECTOR,
      "x"_a.noconvert())
  ;
}