/** 
 *  \file
 *  CtcConstell Python binding
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

#include "tubex_py_Ctc.h"
#include "tubex_CtcConstell.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcConstell_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcConstell(py::module& m, py::class_<Ctc, pyCtc>& static_ctc)
{
  py::class_<CtcConstell> ctc_constell(m, "CtcConstell", static_ctc, "todo");
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