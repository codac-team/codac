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

    .def_static("purple", &Color::purple,
      STATIC_COLOR_COLOR_PURPLE_FLOAT,
      "alpha"_a=1.)

    .def_static("dark_green", &Color::dark_green,
      STATIC_COLOR_COLOR_DARK_GREEN_FLOAT,
      "alpha"_a=1.)
  ;

  py::implicitly_convertible<py::list, Color>();
}