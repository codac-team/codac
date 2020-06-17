/** 
 *  \file
 *  VIBesFig Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "tubex_VIBesFig.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_VIBesFig_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_VIBesFig(py::module& m)
{
  py::class_<vibes::Params> params(m, "Params");

  py::class_<VIBesFig,Figure> vibes_fig(m, "VIBesFig", VIBESFIG_MAIN);
  vibes_fig

  // Definition and properties

    .def(py::init<const string&>(),
      VIBESFIG_VIBESFIG_STRING,
      "fig_name"_a)

    .def("close", &VIBesFig::close,
      VIBESFIG_VOID_CLOSE)

    .def("set_properties", &VIBesFig::set_properties,
      VIBESFIG_VOID_SET_PROPERTIES_INT_INT_INT_INT,
      "x"_a, "y"_a, "width"_a, "height"_a)

    .def("set_background", &VIBesFig::set_background,
      VIBESFIG_VOID_SET_BACKGROUND_STRING,
      "bg_color"_a)

    .def("axis_limits", (const IntervalVector& (VIBesFig::*)(double,double,double,double,bool,float))&VIBesFig::axis_limits,
      VIBESFIG_CONSTINTERVALVECTOR_AXIS_LIMITS_DOUBLE_DOUBLE_DOUBLE_DOUBLE_BOOL_FLOAT,
      "x_min"_a, "x_max"_a, "y_min"_a, "y_max"_a, "same_ratio"_a=false, "margin"_a=0)

    .def("axis_limits", (const IntervalVector& (VIBesFig::*)(const IntervalVector&,bool,float))&VIBesFig::axis_limits,
      VIBESFIG_CONSTINTERVALVECTOR_AXIS_LIMITS_INTERVALVECTOR_BOOL_FLOAT,
      "viewbox"_a, "same_ratio"_a=false, "margin"_a=0)
  
  // Saving this figure

    .def("save_image", &VIBesFig::save_image,
      VIBESFIG_VOID_SAVE_IMAGE_STRING_STRING_STRING,
      "suffix"_a="", "extension"_a="svg", "path"_a=".")
  
  // Figure's content

    .def("show", &VIBesFig::show,
      VIBESFIG_VOID_SHOW)

    .def("clear", &VIBesFig::clear,
      VIBESFIG_VOID_CLEAR)
  
  // Displaying objects

    .def("draw_box", (void (VIBesFig::*)(const IntervalVector &,const string &,const vibes::Params &))&VIBesFig::draw_box,
      VIBESFIG_VOID_DRAW_BOX_INTERVALVECTOR_STRING_VIBESPARAMS,
      "box"_a, "color"_a="black", "params"_a=vibes::Params())

    .def("draw_circle", (void (VIBesFig::*)(double,double,double,const string &,const vibes::Params &))&VIBesFig::draw_circle,
      VIBESFIG_VOID_DRAW_CIRCLE_DOUBLE_DOUBLE_DOUBLE_STRING_VIBESPARAMS,
      "x"_a, "y"_a, "r"_a, "color"_a="black", "params"_a=vibes::Params())

    .def("draw_ring", (void (VIBesFig::*)(double,double,const Interval&,const string &,const vibes::Params &))&VIBesFig::draw_ring,
      VIBESFIG_VOID_DRAW_RING_DOUBLE_DOUBLE_INTERVAL_STRING_VIBESPARAMS,
      "x"_a, "y"_a, "r"_a, "color"_a="black", "params"_a=vibes::Params())

    .def("draw_pie", (void (VIBesFig::*)(double,double,const Interval &,const Interval &,const string &,const vibes::Params &))&VIBesFig::draw_pie,
      VIBESFIG_VOID_DRAW_PIE_DOUBLE_DOUBLE_INTERVAL_INTERVAL_STRING_VIBESPARAMS,
      "x"_a, "y"_a, "r"_a, "theta"_a, "color"_a="black", "params"_a=vibes::Params())
  ;
}