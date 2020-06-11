/** 
 *  \file
 *  CtcDist Python binding
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
#include "tubex_CtcDist.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcDist_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcDist(py::module& m, py::class_<Ctc,pyCtc>& static_ctc)
{
  py::class_<CtcDist> ctc_dist(m, "CtcDist", static_ctc, "todo");
  ctc_dist

    .def(py::init<>(),
      CTCDIST_CTCDIST)

    .def("contract", &CtcDist::contract,
      CTCDIST_VOID_CONTRACT_INTERVALVECTOR_INTERVALVECTOR_INTERVAL,
      "a"_a.noconvert(), "b"_a.noconvert(), "d"_a.noconvert())
  ;
}