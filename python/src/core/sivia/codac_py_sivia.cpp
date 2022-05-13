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


void export_sivia(py::module& m, py::class_<Ctc,pyCtc>& ctc, py::class_<ibex::Sep,pySep>& sep)
{
  m.def("SIVIA", [](const IntervalVector& x, Ctc& ctc, float precision,
    bool display_result, const string& fig_name, bool return_result, const SetColorMap& color_map)
    {
      return SIVIA(x, ctc, precision, display_result, fig_name, return_result, color_map);
    },
    "x"_a.noconvert(), "ctc"_a.noconvert(), "precision"_a.noconvert(), 
    "display_result"_a.noconvert() = true, "fig_name"_a.noconvert() = "", "return_result"_a.noconvert() = false, "color_map"_a.noconvert() = DEFAULT_SET_COLOR_MAP);

  m.def("SIVIA", [](const IntervalVector& x, ibex::Sep& sep, float precision,
    bool display_result, const string& fig_name, bool return_result, const SetColorMap& color_map)
    {
      return SIVIA(x, sep, precision, display_result, fig_name, return_result, color_map);
    },
    "x"_a.noconvert(), "sep"_a.noconvert(), "precision"_a.noconvert(),
    "display_result"_a.noconvert() = true, "fig_name"_a.noconvert() = "", "return_result"_a.noconvert() = false, "color_map"_a.noconvert() = DEFAULT_SET_COLOR_MAP);
}