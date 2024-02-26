/** 
 *  \file
 *  Figure Python binding
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

#include "vibes.h"

using namespace std;
namespace py = pybind11;
using namespace pybind11::literals;


void export_graphics(py::module& m)
{
  m.def("beginDrawing", []() { 
    //cout << "Warning: beginDrawing() is deprecated. Use begin_drawing() instead." << endl;
    vibes::beginDrawing(); });
  m.def("endDrawing",   []() { 
    //cout << "Warning: endDrawing() is deprecated. Use end_drawing() instead." << endl;
    vibes::endDrawing(); });

  //m.def("begin_drawing", []() { vibes::beginDrawing(); });
  //m.def("end_drawing",   []() { vibes::endDrawing(); });
}