/** 
 *  \file
 *  Variable Python binding
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "codac_Variable.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_Variable_docs.h"

using namespace std;
using namespace ibex;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_IntervalVar(py::module& m)
{
  py::class_<IntervalVar> intervalvar(m, "IntervalVar", "todo"); // todo: doc
  intervalvar

    .def(py::init<>(),
      "todo") // todo: doc
  ;
}

void export_IntervalVectorVar(py::module& m)
{
  py::class_<IntervalVectorVar> intervalvectorvar(m, "IntervalVectorVar", "todo"); // todo: doc
  intervalvectorvar

    .def(py::init<int>(),
      "todo",
      "n"_a.noconvert()) // todo: doc
  ;
}