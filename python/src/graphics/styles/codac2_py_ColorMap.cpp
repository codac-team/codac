/** 
 *  Codac binding (graphics)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_ColorMap.h>
#include "codac2_py_ColorMap_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_ColorMap(py::module& m)
{
  py::class_<ColorMap> exported_colormap(m, "ColorMap", COLORMAP_MAIN);
  exported_colormap

    .def(py::init<Model>(),
        COLORMAP_COLORMAP_MODEL,
        "m"_a=Model::RGB)

    .def(
        #if FOR_MATLAB
          "__call__"
        #else
          "__getitem__"
        #endif
        , [](const ColorMap& x, float r) -> const Color&
        {
          return x.at(r);
        }, py::return_value_policy::reference_internal)

    .def(
        #if FOR_MATLAB
          "setitem"
        #else
          "__setitem__"
        #endif
        , [](ColorMap& x, float r, const Color& c)
        {
          x[r] = c;
        })

    .def("model", &ColorMap::model,
        CONST_MODEL_REF_COLORMAP_MODEL_CONST)

    .def("color", &ColorMap::color,
        COLOR_COLORMAP_COLOR_FLOAT_CONST,
        "r"_a)

    // Predifined color maps

    .def_static("haxby", &ColorMap::haxby,
        STATIC_COLORMAP_COLORMAP_HAXBY_FLOAT,
        "alpha"_a=1.)
    
    .def_static("basic", &ColorMap::basic,
        STATIC_COLORMAP_COLORMAP_BASIC_FLOAT,
        "alpha"_a=1.)

    .def_static("blue_tube", &ColorMap::blue_tube,
        STATIC_COLORMAP_COLORMAP_BLUE_TUBE_FLOAT,
        "alpha"_a=1.)


    .def_static("red_tube", &ColorMap::red_tube,
        STATIC_COLORMAP_COLORMAP_RED_TUBE_FLOAT,
        "alpha"_a=1.)

    .def_static("rainbow", &ColorMap::rainbow,
        STATIC_COLORMAP_COLORMAP_RAINBOW_FLOAT,
        "alpha"_a=1.)

  ;
}