/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include <codac2_StyleProperties.h>
#include "codac2_py_StyleProperties_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_StyleProperties(py::module& m)
{
  py::class_<Color> exported_color(m, "Color", COLOR_MAIN);
  exported_color

    .def_readwrite("r", &Color::r)
    .def_readwrite("g", &Color::g)
    .def_readwrite("b", &Color::b)
    .def_readwrite("alpha", &Color::alpha)

    .def(py::init<int,int,int,int>(),
      COLOR_COLOR_INT_INT_INT_INT,
      "r"_a, "g"_a, "b"_a, "alpha"_a=255)

    .def(py::init<float,float,float,float>(),
      COLOR_COLOR_FLOAT_FLOAT_FLOAT_FLOAT,
      "r"_a, "g"_a, "b"_a, "alpha"_a=1.)

    .def("to_hex_str", &Color::to_hex_str,
      STRING_COLOR_TO_HEX_STR_CONST_STRING_REF_CONST,
      "prefix"_a="#")

    // Predefined colors

    .def_static("none", &Color::none,
      STATIC_COLOR_COLOR_NONE)

    .def_static("black", &Color::black,
      STATIC_COLOR_COLOR_BLACK_FLOAT,
      "alpha"_a=1.)

    .def_static("white", &Color::white,
      STATIC_COLOR_COLOR_WHITE_FLOAT,
      "alpha"_a=1.)

    .def_static("green", &Color::green,
      STATIC_COLOR_COLOR_GREEN_FLOAT,
      "alpha"_a=1.)

    .def_static("blue", &Color::blue,
      STATIC_COLOR_COLOR_BLUE_FLOAT,
      "alpha"_a=1.)

    .def_static("cyan", &Color::cyan,
      STATIC_COLOR_COLOR_CYAN_FLOAT,
      "alpha"_a=1.)

    .def_static("yellow", &Color::yellow,
      STATIC_COLOR_COLOR_YELLOW_FLOAT,
      "alpha"_a=1.)

    .def_static("red", &Color::red,
      STATIC_COLOR_COLOR_RED_FLOAT,
      "alpha"_a=1.)

    .def_static("dark_gray", &Color::dark_gray,
      STATIC_COLOR_COLOR_DARK_GRAY_FLOAT,
      "alpha"_a=1.)
  ;

  py::class_<StyleProperties> exported(m, "StyleProperties", STYLEPROPERTIES_MAIN);
  exported
  
    .def(py::init<>(),
      STYLEPROPERTIES_STYLEPROPERTIES)
  
    .def(py::init<const Color&>(),
      STYLEPROPERTIES_STYLEPROPERTIES_CONST_COLOR_REF,
      "color"_a)

    .def(py::init(
        [](const std::vector<Color>& v)
        {
          if(v.size() == 1)
            return std::make_unique<StyleProperties>(v[0]);
          else if(v.size() == 2)
            return std::make_unique<StyleProperties,std::initializer_list<Color>>({ v[0], v[1] });
          else
          {
            throw invalid_argument("StyleProperties must be built from one (edge) or two (edge/fill) colors.");
            return std::make_unique<StyleProperties>();
          }
        }),
      STYLEPROPERTIES_STYLEPROPERTIES_INITIALIZER_LIST_COLOR,
      "v"_a)
  
    .def(py::init<const std::string&>(),
      STYLEPROPERTIES_STYLEPROPERTIES_CONST_STRING_REF,
      "vibes_style"_a)

    .def_static("inside", &StyleProperties::inside,
      STATIC_STYLEPROPERTIES_STYLEPROPERTIES_INSIDE)

    .def_static("outside", &StyleProperties::outside,
      STATIC_STYLEPROPERTIES_STYLEPROPERTIES_OUTSIDE)

    .def_static("boundary", &StyleProperties::boundary,
      STATIC_STYLEPROPERTIES_STYLEPROPERTIES_BOUNDARY)

  ;
  
  py::implicitly_convertible<Color,StyleProperties>();
  py::implicitly_convertible<py::list,StyleProperties>();
}