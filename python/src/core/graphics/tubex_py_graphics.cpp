
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


//PYBIND11_MODULE(graphics, m){

void export_graphics(py::module& m){
    m.def("beginDrawing", [](){vibes::beginDrawing();});
    m.def("endDrawing", [](){vibes::endDrawing();});
}