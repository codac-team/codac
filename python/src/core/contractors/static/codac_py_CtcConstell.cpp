/** 
 *  \file
 *  CtcConstell Python binding
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
#include "codac_CtcConstell.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcConstell_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcConstell(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
  py::class_<CtcConstell> ctc_constell(m, "CtcConstell", ctc, CTCCONSTELL_MAIN);
  ctc_constell

    .def(py::init<const list<IntervalVector> &>(),
      CTCCONSTELL_CTCCONSTELL_VECTORINTERVALVECTOR,
      "map"_a.noconvert())

    .def(py::init<const vector<IntervalVector> &>(),
      CTCCONSTELL_CTCCONSTELL_VECTORINTERVALVECTOR,
      "map"_a.noconvert())

    .def("contract", &CtcConstell::contract,
      CTCCONSTELL_VOID_CONTRACT_INTERVALVECTOR,
      "beacon_box"_a.noconvert())
  ;
}