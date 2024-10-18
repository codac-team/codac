/** 
 *  \file
 *  CtcDist Python binding
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
#include "codac_CtcDist.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcDist_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcDist(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
  py::class_<CtcDist> ctc_dist(m, "CtcDist", ctc, CTCDIST_MAIN);
  ctc_dist

    .def(py::init<>(),
      CTCDIST_CTCDIST)

    .def("contract", (void (CtcDist::*)(IntervalVector&))&CtcDist::contract,
      CTCDIST_VOID_CONTRACT_INTERVALVECTOR,
      "x"_a.noconvert())

    .def("contract", (void (CtcDist::*)(IntervalVector&,IntervalVector&,Interval&))&CtcDist::contract,
      CTCDIST_VOID_CONTRACT_INTERVALVECTOR_INTERVALVECTOR_INTERVAL,
      "a"_a.noconvert(), "b"_a.noconvert(), "d"_a.noconvert())

    .def("contract", (void (CtcDist::*)(Interval&,Interval&,Interval&,Interval&,Interval&))&CtcDist::contract,
      CTCDIST_VOID_CONTRACT_INTERVAL_INTERVAL_INTERVAL_INTERVAL_INTERVAL,
      "ax"_a.noconvert(), "ay"_a.noconvert(), "bx"_a.noconvert(), "by"_a.noconvert(), "d"_a.noconvert())
  ;
}