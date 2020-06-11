/** 
 *  \file
 *  VIBesFigTubeVector Python binding
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

#include "tubex_VIBesFigTubeVector.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_VIBesFigTubeVector_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_VIBesFigTubeVector(py::module& m)
{
  py::class_<VIBesFigTubeVector,Figure> fig_tubevector(m, "VIBesFigTubeVector", "todo");
  fig_tubevector

  // Basics

    .def(py::init<const string&>(),
      VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING,
      "fig_name"_a)

    .def(py::init<const string&,int,int>(),
      VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING_INT_INT,
      "fig_name"_a, "start_index"_a, "end_index"_a)

    .def(py::init<const string&,const TubeVector*,const TrajectoryVector*>(),
      VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING_TUBEVECTOR_TRAJECTORYVECTOR,
      "fig_name"_a, "tubevector"_a, "trajvector"_a=nullptr)

    .def("size", &VIBesFigTubeVector::size,
      VIBESFIGTUBEVECTOR_INT_SIZE)

    .def("subfigs_number", &VIBesFigTubeVector::subfigs_number,
      VIBESFIGTUBEVECTOR_INT_SUBFIGS_NUMBER)

    .def("set_properties", &VIBesFigTubeVector::set_properties,
      VIBESFIGTUBEVECTOR_VOID_SET_PROPERTIES_INT_INT_INT_INT,
      "x"_a, "y"_a, "width"_a, "height"_a)

    .def("show", &VIBesFigTubeVector::show,
      VIBESFIGTUBEVECTOR_VOID_SHOW_BOOL,
      "detail_slices"_a=false)

    .def("set_cursor", &VIBesFigTubeVector::set_cursor,
      VIBESFIGTUBEVECTOR_VOID_SET_CURSOR_DOUBLE,
      "t"_a)

    .def("show_cursor", &VIBesFigTubeVector::show_cursor,
      VIBESFIGTUBEVECTOR_VOID_SHOW_CURSOR_BOOL,
      "display"_a=true)

  // Simple drawings

    .def("draw_box", (void (VIBesFigTubeVector::*)(const Interval&,const IntervalVector&,const vibes::Params&))&VIBesFigTubeVector::draw_box,
      VIBESFIGTUBEVECTOR_VOID_DRAW_BOX_INTERVAL_INTERVALVECTOR_VIBESPARAMS,
      "domain"_a, "box"_a, "params"_a)

    .def("draw_box", (void (VIBesFigTubeVector::*)(const Interval&,const IntervalVector&,const string&,const vibes::Params&))&VIBesFigTubeVector::draw_box,
      VIBESFIGTUBEVECTOR_VOID_DRAW_BOX_INTERVAL_INTERVALVECTOR_STRING_VIBESPARAMS,
      "domain"_a, "box"_a, "color"_a="", "params"_a=vibes::Params())

  // Handling tubes

    .def("add_tube", &VIBesFigTubeVector::add_tube,
      VIBESFIGTUBEVECTOR_VOID_ADD_TUBE_TUBEVECTOR_STRING_STRING_STRING,
      "tubevector"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)

    .def("set_tube_name", &VIBesFigTubeVector::set_tube_name,
      VIBESFIGTUBEVECTOR_VOID_SET_TUBE_NAME_TUBEVECTOR_STRING,
      "tubevector"_a, "name"_a)

    .def("set_tube_derivative", &VIBesFigTubeVector::set_tube_derivative,
      VIBESFIGTUBEVECTOR_VOID_SET_TUBE_DERIVATIVE_TUBEVECTOR_TUBEVECTOR,
      "tubevector"_a, "derivative"_a)

    .def("set_tube_color", (void (VIBesFigTubeVector::*)(const TubeVector*,const string&,const string&))&VIBesFigTubeVector::set_tube_color,
      VIBESFIGTUBEVECTOR_VOID_SET_TUBE_COLOR_TUBEVECTOR_STRING_STRING,
      "tubevector"_a, "color_frgrnd"_a, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)

    .def("set_tube_color", (void (VIBesFigTubeVector::*)(const TubeVector*,TubeColorType,const string&))&VIBesFigTubeVector::set_tube_color,
      VIBESFIGTUBEVECTOR_VOID_SET_TUBE_COLOR_TUBEVECTOR_TUBECOLORTYPE_STRING,
      "tubevector"_a, "color_type"_a, "color"_a)

    .def("reset_tube_background", &VIBesFigTubeVector::reset_tube_background,
      VIBESFIGTUBEVECTOR_VOID_RESET_TUBE_BACKGROUND_TUBEVECTOR,
      "tubevector"_a)

    .def("remove_tube", &VIBesFigTubeVector::remove_tube,
      VIBESFIGTUBEVECTOR_VOID_REMOVE_TUBE_TUBEVECTOR,
      "tubevector"_a)

  // Handling trajectories

    .def("add_trajectory", &VIBesFigTubeVector::add_trajectory,
      VIBESFIGTUBEVECTOR_VOID_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_STRING,
      "trajvector"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR)

    .def("set_trajectory_name", &VIBesFigTubeVector::set_trajectory_name,
      VIBESFIGTUBEVECTOR_VOID_SET_TRAJECTORY_NAME_TRAJECTORYVECTOR_STRING,
      "trajvector"_a, "name"_a)

    .def("set_trajectory_color", &VIBesFigTubeVector::set_trajectory_color,
      VIBESFIGTUBEVECTOR_VOID_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_STRING,
      "trajvector"_a, "color"_a)

    .def("remove_trajectory", &VIBesFigTubeVector::remove_trajectory,
      VIBESFIGTUBEVECTOR_VOID_REMOVE_TRAJECTORY_TRAJECTORYVECTOR,
      "trajvector"_a)

  // Python vector methods

    .def("__len__", &VIBesFigTubeVector::subfigs_number)

  ;
}