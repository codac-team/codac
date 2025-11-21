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
#include <codac2_StyleProperties.h>
#include "codac2_py_StyleProperties_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_StyleProperties(py::module& m)
{
  py::class_<StyleProperties> exported(m, "StyleProperties", STYLEPROPERTIES_MAIN);
  exported
  
    .def(py::init<>(),
      STYLEPROPERTIES_STYLEPROPERTIES)

    .def(py::init<const std::string&, const std::string&, const std::string&>(),
      STYLEPROPERTIES_STYLEPROPERTIES_CONST_STRING_REF_CONST_STRING_REF_CONST_STRING_REF,
      "param1"_a, "param2"_a="", "param3"_a="")
  
    .def(py::init<const Color&, const std::string&, const std::string&, const std::string&>(),
      STYLEPROPERTIES_STYLEPROPERTIES_CONST_COLOR_REF_CONST_STRING_REF_CONST_STRING_REF_CONST_STRING_REF,
      "stroke_color"_a, "param1"_a="", "param2"_a="", "param3"_a="")

    .def(py::init(
        [](const std::vector<Color>& v, const std::string& param1, const std::string& param2, const std::string& param3)
        {
          if(v.size() == 1)
            return std::make_unique<StyleProperties>(v[0], param1, param2, param3);
          else if(v.size() == 2)
            return std::make_unique<StyleProperties,std::initializer_list<Color>>({ v[0], v[1] }, param1, param2, param3);
          else
          {
            throw invalid_argument("StyleProperties must be built from one (edge) or two (edge/fill) colors.");
            return std::make_unique<StyleProperties>();
          }
        }),
      STYLEPROPERTIES_STYLEPROPERTIES_INITIALIZER_LIST_COLOR_CONST_STRING_REF_CONST_STRING_REF_CONST_STRING_REF,
      "v"_a, "param1"_a="", "param2"_a="", "param3"_a="")


    .def_static("inside", &StyleProperties::inside,
      STATIC_STYLEPROPERTIES_STYLEPROPERTIES_INSIDE)

    .def_static("outside", &StyleProperties::outside,
      STATIC_STYLEPROPERTIES_STYLEPROPERTIES_OUTSIDE)

    .def_static("boundary", &StyleProperties::boundary,
      STATIC_STYLEPROPERTIES_STYLEPROPERTIES_BOUNDARY)

  ;
  
  py::implicitly_convertible<py::list,StyleProperties>();
  py::implicitly_convertible<Color,StyleProperties>();
}