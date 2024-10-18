/** 
 *  \file
 *  BoolInterval Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include "codac_type_caster.h"

#include "codac_BoolInterval.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
// todo: #include "codac_py_BoolInterval_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_BoolInterval(py::module& m)
{
  py::enum_<BoolInterval>(m, "BoolInterval")
    .value("YES", BoolInterval::YES)
    .value("MAYBE", BoolInterval::MAYBE)
    .value("NO", BoolInterval::NO)
    .value("EMPTY_BOOL", BoolInterval::EMPTY_BOOL)
  ;
}