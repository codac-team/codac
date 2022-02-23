/** 
 *  \file
 *  Paving Python binding
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

#include "codac_Paving.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_Paving_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Paving(py::module& m)
{
  py::class_<Paving> tplane(m, "Paving", PAVING_MAIN);
}