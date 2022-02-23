/** 
 *  \file
 *  Ctc Python binding
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
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_Ctc_docs.h" // todo: generate this file from Doxygen doc

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


CtcUnion* __or(Ctc& c1, Ctc& c2)
{
  return new CtcUnion(c1, c2);
  // todo: manage delete
}

CtcCompo* __and(Ctc& c1, Ctc& c2)
{
  return new CtcCompo(c1, c2);
  // todo: manage delete
}

py::class_<Ctc,pyCtc> export_Ctc(py::module& m)
{
  // Export Ctc
  py::class_<Ctc,pyCtc> ctc(m, "Ctc", DOCS_CTC_TYPE);
  ctc

    .def(py::init<int>())
    .def_readonly("nb_var", &Ctc::nb_var, DOC_CTC_NB_VAR)

    .def("__or__", &__or,
      DOC_CTC_OR,
      py::return_value_policy::take_ownership,
      py::keep_alive<0,1>(), py::keep_alive<0,2>())

    .def("__and__", &__and,
      DOC_CTC_AND,
      py::return_value_policy::take_ownership,
      py::keep_alive<0,1>(), py::keep_alive<0,2>())
    ;

  // Export CtcUnion
  py::class_<CtcUnion>(m, "CtcUnion", ctc, DOC_CTCUNION_TYPE)
    .def(py::init<ibex::Array<Ctc>>(), py::keep_alive<1,2>(), "list"_a)
    .def("contract", (void (Ctc::*)(IntervalVector&)) &CtcUnion::contract)
    ;

  // Export CtcCompo
  py::class_<CtcCompo>(m, "CtcCompo", ctc, DOC_CTCCOMPO_TYPE)
    .def(py::init<ibex::Array<Ctc>>(), py::keep_alive<1,2>(), "list"_a)
    .def("contract", (void (Ctc::*)(IntervalVector&)) &CtcCompo::contract)
    ;

  return ctc;
}