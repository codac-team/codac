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
#include <codac2_PavingStyle.h>
#include "codac2_py_PavingStyle_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_PavingStyle(py::module& m)
{
  py::class_<PavingStyle> exported(m, "PavingStyle", PAVINGSTYLE_MAIN);
  exported
  
    .def(py::init<>(),
      PAVINGSTYLE_PAVINGSTYLE)
  
    .def(py::init<const StyleProperties&,const StyleProperties&,const StyleProperties&>(),
      PAVINGSTYLE_PAVINGSTYLE_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "boundary"_a, "outside"_a, "inside"_a)

    .def_readwrite("boundary", &PavingStyle::boundary,
      STYLEPROPERTIES_PAVINGSTYLE_BOUNDARY)

    .def_readwrite("outside", &PavingStyle::outside,
      STYLEPROPERTIES_PAVINGSTYLE_OUTSIDE)

    .def_readwrite("inside", &PavingStyle::inside,
      STYLEPROPERTIES_PAVINGSTYLE_INSIDE)
    
    .def_static("default_style", &PavingStyle::default_style,
      STATIC_PAVINGSTYLE_PAVINGSTYLE_DEFAULT_STYLE)

    .def_static("blue_green", &PavingStyle::blue_green,
      STATIC_PAVINGSTYLE_PAVINGSTYLE_BLUE_GREEN)

    .def_static("blue_white", &PavingStyle::blue_white,
      STATIC_PAVINGSTYLE_PAVINGSTYLE_BLUE_WHITE)

    .def_static("blue_pink", &PavingStyle::blue_pink,
      STATIC_PAVINGSTYLE_PAVINGSTYLE_BLUE_PINK)

    .def_static("black_white", &PavingStyle::black_white,
      STATIC_PAVINGSTYLE_PAVINGSTYLE_BLACK_WHITE)

  ;
}