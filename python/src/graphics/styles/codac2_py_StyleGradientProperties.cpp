/** 
 *  Codac binding (graphics)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_StyleGradientProperties.h>
#include "codac2_py_StyleGradientProperties_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_StyleGradientProperties(py::module& m)
{
  py::class_<StyleGradientProperties> exported(m, "StyleGradientProperties", STYLEGRADIENTPROPERTIES_MAIN);
  exported
  
    .def(py::init<>(),
      STYLEGRADIENTPROPERTIES_STYLEGRADIENTPROPERTIES)

    .def(py::init<const std::string&, const std::string&, const std::string&, const std::string&>(),
      STYLEGRADIENTPROPERTIES_STYLEGRADIENTPROPERTIES_CONST_STRING_REF_CONST_STRING_REF_CONST_STRING_REF_CONST_STRING_REF,
      "param1"_a, "param2"_a="", "param3"_a="", "param4"_a="")
  
    .def(py::init<const ColorMap&, const std::string&, const std::string&, const std::string&, const std::string&>(),
      STYLEGRADIENTPROPERTIES_STYLEGRADIENTPROPERTIES_CONST_COLORMAP_REF_CONST_STRING_REF_CONST_STRING_REF_CONST_STRING_REF_CONST_STRING_REF,
      "cmap"_a, "param1"_a="", "param2"_a="", "param3"_a="", "param4"_a="")

  ;
  
  py::implicitly_convertible<ColorMap,StyleGradientProperties>();
}