/** 
 *  \file
 *  Figure Python binding
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

#include "vibes.h"

using namespace std;
namespace py = pybind11;
using namespace pybind11::literals;


void export_graphics(py::module& m)
{
  m.def("beginDrawing", []() { vibes::beginDrawing(); });
  m.def("endDrawing",   []() { vibes::endDrawing(); });
}