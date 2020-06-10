
/** 
 *  \file
 *  VIBesFig binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Figure.h"
#include "tubex_VIBesFig.h"
#include "tubex_VIBesFigTube.h"
#include "tubex_VIBesFigTubeVector.h"
#include "pyIbex_type_caster.h"

#include "tubex_VIBesFigMap.h"

// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_ColorMap_docs.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;


void export_ColorMap(py::module& m){

    py::class_<ColorMap>(m, "ColorMap")
        .def("is_opaque", &ColorMap::is_opaque, COLORMAP_BOOL_IS_OPAQUE)

    ;
    // bool is_opaque() const;
    // py::class_<FigMapTubeParams> figmaptubeparams(m, "FigMapTubeParams" DOCS_FIGMAPTUBEPARAMS);
    // py::class_<FigMapTrajParams> figmaptrajparams(m, "FigMapTrajParams" DOCS_FIGMAPTRAJPARAMS);

}