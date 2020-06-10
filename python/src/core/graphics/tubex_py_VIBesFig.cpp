
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

// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_VIBesFig_docs.h"

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


void export_VIBesFig(py::module& m){


    py::class_<vibes::Params> params(m, "Params");

    py::class_<VIBesFig> vibes_fig(m, "VIBesFig", "todo");

    vibes_fig
      .def(py::init<const std::string &>(),
          VIBESFIG_VIBESFIG_STRING, "fig_name"_a)
      .def("close", &VIBesFig::close,VIBESFIG_VOID_CLOSE)
      .def("set_properties", &VIBesFig::set_properties,
          VIBESFIG_VOID_SET_PROPERTIES_INT_INT_INT_INT, "x"_a, "y"_a, "width"_a, "height"_a)
      .def("set_background", &VIBesFig::set_background,
          VIBESFIG_VOID_SET_BACKGROUND_STRING, "bg_color"_a)
      .def("axis_limits", (const ibex::IntervalVector & (VIBesFig::*)(double,double,double,double,bool,float) )&VIBesFig::axis_limits,
          VIBESFIG_CONSTINTERVALVECTOR_AXIS_LIMITS_DOUBLE_DOUBLE_DOUBLE_DOUBLE_BOOL_FLOAT, "x_min"_a, "x_max"_a, "y_min"_a, "y_max"_a, "same_ratio"_a=false, "margin"_a=0)
      .def("axis_limits", (const ibex::IntervalVector & (VIBesFig::*)(const ibex::IntervalVector &,bool,float) )&VIBesFig::axis_limits,
          VIBESFIG_CONSTINTERVALVECTOR_AXIS_LIMITS_INTERVALVECTOR_BOOL_FLOAT, "viewbox"_a, "same_ratio"_a=false, "margin"_a=0)
      .def("save_image", &VIBesFig::save_image,
          VIBESFIG_VOID_SAVE_IMAGE_STRING_STRING_STRING, "suffix"_a="", "extension"_a="svg", "path"_a=".")
      .def("show", &VIBesFig::show,VIBESFIG_VOID_SHOW)
      .def("clear", &VIBesFig::clear,VIBESFIG_VOID_CLEAR)
      .def("draw_box", (void (VIBesFig::*)(const ibex::IntervalVector &,const std::string &,const vibes::Params &))&VIBesFig::draw_box,
           VIBESFIG_VOID_DRAW_BOX_INTERVALVECTOR_STRING_VIBESPARAMS, "box"_a, "color"_a = "black", "params"_a=vibes::Params())
      .def("draw_circle", (void (VIBesFig::*)(double,double,double,const std::string &,const vibes::Params &))&VIBesFig::draw_circle,
           VIBESFIG_VOID_DRAW_CIRCLE_DOUBLE_DOUBLE_DOUBLE_STRING_VIBESPARAMS, "x"_a, "y"_a, "r"_a, "color"_a = "black", "params"_a=vibes::Params())
      .def("draw_ring", (void (VIBesFig::*)(double,double,const ibex::Interval&,const std::string &,const vibes::Params &))&VIBesFig::draw_ring,
           VIBESFIG_VOID_DRAW_RING_DOUBLE_DOUBLE_INTERVAL_STRING_VIBESPARAMS, "x"_a, "y"_a, "r"_a, "color"_a = "black", "params"_a=vibes::Params())
      .def("draw_pie", (void (VIBesFig::*)(double,double,const ibex::Interval &,const ibex::Interval &,const std::string &,const vibes::Params &))&VIBesFig::draw_pie,
           VIBESFIG_VOID_DRAW_PIE_DOUBLE_DOUBLE_INTERVAL_INTERVAL_STRING_VIBESPARAMS, "x"_a, "y"_a, "r"_a, "theta"_a, "color"_a = "black", "params"_a=vibes::Params())
    //   .def("draw_box", (void (VIBesFigTubeVector::*)(const ibex::Interval &,const ibex::IntervalVector &,const std::string &,const vibes::Params &) )&VIBesFigTubeVector::draw_box,
    //       DOCS_VIBESFIGTUBEVECTOR_DRAW_BOX_INTERVAL_INTERVALVECTOR_STRING_VIBES::PARAMS, "domain"_a, "box"_a, "color"_a="", "params"_a=vibes::Params())
    ;
}