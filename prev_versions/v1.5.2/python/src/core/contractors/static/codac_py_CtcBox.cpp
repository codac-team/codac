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

#include "codac_py_Ctc.h"
#include "codac_CtcBox.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcBox_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcBox(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
  py::class_<CtcBox> ctc_constell(m, "CtcBox", ctc, CTCBOX_MAIN);
  ctc_constell

    .def(py::init<const IntervalVector&>(),
      CTCBOX_CTCBOX_INTERVALVECTOR,
      "b"_a.noconvert())

    .def("contract", &CtcBox::contract,
      CTCBOX_VOID_CONTRACT_INTERVALVECTOR,
      "x"_a.noconvert())
  ;
}