/** 
 *  \file
 *  CtcBox Python binding
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

#include "../../contractors/static/codac_py_Ctc.h"
#include "codac2_CtcAction.h"

using namespace std;
using namespace codac;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcAction(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
  py::class_<CtcAction> ctc_action(m, "CtcAction", ctc, "todo");
  ctc_action

    .def(py::init<Ctc&,const OctaSym&>(),
      "todo",
      "ctc"_a, "s"_a)

    .def("contract", &CtcAction::contract,
      "todo",
      "x"_a.noconvert())
  ;
}