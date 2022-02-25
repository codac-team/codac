/** 
 *  \file
 *  ColorMap Python binding
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

#include "codac_ColorMap.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_ColorMap_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_ColorMap(py::module& m)
{
  py::class_<ColorMap> color_map(m, "ColorMap", COLORMAP_MAIN);
	color_map

    .def("is_opaque", &ColorMap::is_opaque,
      COLORMAP_BOOL_IS_OPAQUE)
  ;
}