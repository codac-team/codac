/** 
 *  \file
 *  Ctc Python binding
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

using namespace std;
using namespace ibex;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<Ctc,pyCtc> export_Ctc(py::module& m)
{
  return py::class_<Ctc,pyCtc>(m, "Ctc", py::module_local());
}