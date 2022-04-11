/** 
 *  \file
 *  Sep Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Benoît Desrochers, Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "../contractors/static/codac_py_Ctc.h"
#include "../separators/codac_py_Sep.h"

#include <codac_sivia.h>

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_unsupported_geometry(py::module& m, py::class_<Ctc,pyCtc>& ctc, py::class_<ibex::Sep,pySep>& sep)
{
  m.def("SIVIA", [](const IntervalVector& x, Ctc& ctc, float precision, const SetColorMap& color_map)
    {
      SIVIA(x, &ctc, precision, color_map);
    },
    "x"_a.noconvert(), "ctc"_a.noconvert(), "precision"_a.noconvert(), "color_map"_a.noconvert() = DEFAULT_SET_COLOR_MAP);

  m.def("SIVIA", [](const IntervalVector& x, ibex::Sep& sep, float precision, const SetColorMap& color_map)
    {
      SIVIA(x, &sep, precision, color_map);
    },
    "x"_a.noconvert(), "sep"_a.noconvert(), "precision"_a.noconvert(), "color_map"_a.noconvert() = DEFAULT_SET_COLOR_MAP);
}