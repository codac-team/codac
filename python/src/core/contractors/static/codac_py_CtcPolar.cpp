/** 
 *  \file
 *  CtcPolar Python binding
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

#include "codac_py_Ctc.h"
#include "codac_CtcPolar.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
// todo: #include "codac_py_CtcPolar_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcPolar(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
  py::class_<CtcPolar> ctc_polar(m, "CtcPolar", ctc);
  ctc_polar

    .def(py::init<>(),
      "todo")

    .def("contract", (void (CtcPolar::*)(IntervalVector&))&CtcPolar::contract,
      "todo",
      "x"_a.noconvert())

    .def("contract", (void (CtcPolar::*)(Interval&,Interval&,Interval&,Interval&))&CtcPolar::contract,
      "todo",
      "x"_a.noconvert(), "y"_a.noconvert(), "rho"_a.noconvert(), "theta"_a.noconvert())
  ;
}