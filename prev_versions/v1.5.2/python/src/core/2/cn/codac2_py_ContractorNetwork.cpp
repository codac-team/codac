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

#include "codac2_ContractorNetwork.h"

using namespace std;
using namespace codac;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_ContractorNetwork_codac2(py::module& m)
{
  /*py::class_<codac2::ContractorNetwork> cn(m, "ContractorNetwork2", "todo");
  cn

  // Definition

    .def(py::init<>(),
      "todo")

    .def("add", &ContractorNetwork::add,
      "todo",
      "n"_a)

    .def("contract", &ContractorNetwork::contract,
      "todo",
      "verbose"_a=true)
  ;*/
}