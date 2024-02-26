/** 
 *  \file
 *  Python binding
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

#include "codac2_Ctc.h"

using namespace std;
using namespace codac;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Contractor_codac2(py::module& m)
{
  /*py::class_<codac2::ContractorNodeBase,
    std::shared_ptr<ContractorNodeBase> // this is needed to handle shared_ptr with pybind11
  > n(m, "ContractorNodeBase", "todo");

  py::class_<codac2::IntervalVector_<3>> x(m, "IntervalVector_3", "todo");
  x
    .def(py::init<>(),
      "todo")
    .def("print", [](codac2::IntervalVector_<3>& x) { cout << x << endl; }, 
      "todo")
  ;

  py::class_<codac2::Contractor1> ctc(m, "Contractor1", "todo");
  ctc

  // Definition

    .def(py::init<>(),
      "todo")

    .def("__call__", [](codac2::Contractor1& ctc,IntervalVector_<3>& x) { return ctc(x); }, 
      "todo",
      py::return_value_policy::reference_internal)
  ;*/
}