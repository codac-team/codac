/** 
 *  \file
 *  CtcDelay Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_py_DynCtc.h"
#include "codac_CtcDelay.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcDelay_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcDelay(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc)
{
  py::class_<CtcDelay> ctc_eval(m, "CtcDelay", dyn_ctc, CTCDELAY_MAIN);
  ctc_eval

    .def(py::init<>(),
      CTCDELAY_CTCDELAY)

    .def("contract", (void (CtcDelay::*)(Interval&,Tube&,Tube&))&CtcDelay::contract,
      CTCDELAY_VOID_CONTRACT_INTERVAL_TUBE_TUBE,
      "a"_a.noconvert(), "x"_a.noconvert(), "y"_a.noconvert())

    .def("contract", (void (CtcDelay::*)(Interval&,TubeVector&,TubeVector&))&CtcDelay::contract,
      CTCDELAY_VOID_CONTRACT_INTERVAL_TUBEVECTOR_TUBEVECTOR,
      "a"_a.noconvert(), "x"_a.noconvert(), "y"_a.noconvert())
  ;
}