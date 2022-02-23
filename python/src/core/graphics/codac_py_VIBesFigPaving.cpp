/** 
 *  \file
 *  VIBesFigPaving Python binding
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

#include "codac_VIBesFigPaving.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_VIBesFigPaving_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_VIBesFigPaving(py::module& m)
{
  py::class_<VIBesFigPaving,VIBesFig> fig_map(m, "VIBesFigPaving", VIBESFIGPAVING_MAIN);
  fig_map

  .def(py::init<const string&, const Paving*>(),
    VIBESFIGPAVING_CONSTPAVING_M_PAVING,
    "fig_name"_a, "paving"_a)

  .def("show", (void (VIBesFigPaving::*)())&VIBesFigPaving::show,
    VIBESFIGPAVING_VOID_SHOW)

  ;
}