/** 
 *  Codac binding (graphics)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Color.h>
#include <codac2_StyleProperties.h>
#include "codac2_py_Color_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Color(py::module& m)
{

  py::enum_<Model>(m, "Model")
    .value("RGB", Model::RGB)
    .value("HSV", Model::HSV)
  ;

  py::class_<Color> exported_color(m, "Color", COLOR_MAIN);
  exported_color
    
    .def(py::init<>(),COLOR_COLOR)

    .def(py::init<const std::array<float,3>&,Model>(),
      COLOR_COLOR_CONST_ARRAY_FLOAT3_REF_MODEL,
      "xyz"_a, "m_"_a=Model::RGB)

    .def(py::init<const std::array<float,4>&,Model>(),
      COLOR_COLOR_CONST_ARRAY_FLOAT4_REF_MODEL,
      "xyza"_a, "m_"_a=Model::RGB)

    .def(py::init<const std::string&>(),
      COLOR_COLOR_CONST_STRING_REF,
      "hex_str"_a)

    .def("model", &Color::model,
      CONST_MODEL_REF_COLOR_MODEL_CONST)


    // Other formats

    .def("hex_str", &Color::hex_str,
      STRING_COLOR_HEX_STR_CONST)

    .def("vec", &Color::vec,
      VECTOR_COLOR_VEC_CONST)

    // Conversions

    .def("rgb", &Color::rgb,
      COLOR_COLOR_RGB_CONST)

    .def("hsv", &Color::hsv,
      COLOR_COLOR_HSV_CONST)

    // Overload flux operator

    .def("__str__", [](const Color& c) {
      std::ostringstream oss;
      oss << c;
      return oss.str();
    })

    // Predefined colors

    .def_static("none", &Color::none)
    .def_static("black", &Color::black, "alpha"_a=1)
    .def_static("white", &Color::white, "alpha"_a=1)

    .def_static("light_gray", &Color::light_gray, "alpha"_a=1)
    .def_static("gray", &Color::gray, "alpha"_a=1)
    .def_static("dark_gray", &Color::dark_gray, "alpha"_a=1)

    .def_static("light_green", &Color::light_green, "alpha"_a=1)
    .def_static("green", &Color::green, "alpha"_a=1)
    .def_static("dark_green", &Color::dark_green, "alpha"_a=1)

    .def_static("light_blue", &Color::light_blue, "alpha"_a=1)
    .def_static("blue", &Color::blue, "alpha"_a=1)
    .def_static("dark_blue", &Color::dark_blue, "alpha"_a=1)

    .def_static("light_cyan", &Color::light_cyan, "alpha"_a=1)
    .def_static("cyan", &Color::cyan, "alpha"_a=1)
    .def_static("dark_cyan", &Color::dark_cyan, "alpha"_a=1)

    .def_static("light_yellow", &Color::light_yellow, "alpha"_a=1)
    .def_static("yellow", &Color::yellow, "alpha"_a=1)
    .def_static("dark_yellow", &Color::dark_yellow, "alpha"_a=1)

    .def_static("light_orange", &Color::light_orange, "alpha"_a=1)
    .def_static("orange", &Color::orange, "alpha"_a=1)
    .def_static("dark_orange", &Color::dark_orange, "alpha"_a=1)

    .def_static("light_red", &Color::light_red, "alpha"_a=1)
    .def_static("red", &Color::red, "alpha"_a=1)
    .def_static("dark_red", &Color::dark_red, "alpha"_a=1)

    .def_static("light_brown", &Color::light_brown, "alpha"_a=1)
    .def_static("brown", &Color::brown, "alpha"_a=1)
    .def_static("dark_brown", &Color::dark_brown, "alpha"_a=1)

    .def_static("light_purple", &Color::light_purple, "alpha"_a=1)
    .def_static("purple", &Color::purple, "alpha"_a=1)
    .def_static("dark_purple", &Color::dark_purple, "alpha"_a=1)

    .def_static("light_pink", &Color::light_pink, "alpha"_a=1)
    .def_static("pink", &Color::pink, "alpha"_a=1)
    .def_static("dark_pink", &Color::dark_pink, "alpha"_a=1)
  ;

  py::implicitly_convertible<py::list, Color>();
}