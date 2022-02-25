/** 
 *  \file
 *  DynCtc Python binding
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
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_DynCtc_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<DynCtc,pyDynCtc> export_DynCtc(py::module& m)
{
  py::class_<DynCtc,pyDynCtc> dyn_ctc(m, "DynCtc", DYNCTC_MAIN);
  dyn_ctc

    .def(py::init<>(),
      DYNCTC_DYNCTC_BOOL)

    .def("preserve_slicing", &DynCtc::preserve_slicing,
      DYNCTC_VOID_PRESERVE_SLICING_BOOL,
      "preserve"_a=true)

    .def("set_fast_mode", &DynCtc::set_fast_mode,
      DYNCTC_VOID_SET_FAST_MODE_BOOL,
      "fast_mode"_a=true)

    .def("restrict_tdomain", &DynCtc::restrict_tdomain,
      DYNCTC_VOID_RESTRICT_TDOMAIN_INTERVAL,
      "tdomain"_a)
    ;

  py::enum_<TimePropag>(m, "TimePropag")
    .value("FORWARD", TimePropag::FORWARD)
    .value("BACKWARD", TimePropag::BACKWARD)
  ;

  return dyn_ctc;
}