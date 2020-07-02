/** 
 *  \file
 *  VIBesFigTube Python binding
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

#include "tubex_VIBesFigTube.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_VIBesFigTube_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_VIBesFigTube(py::module& m)
{
  py::class_<VIBesFigTube,VIBesFig> fig_tube(m, "VIBesFigTube", VIBESFIGTUBE_MAIN);
  fig_tube

  // Basics

    .def(py::init<const string&,const Tube*,const Trajectory*>(),
      VIBESFIGTUBE_VIBESFIGTUBE_STRING_TUBE_TRAJECTORY,
      "fig_name"_a, "tube"_a=nullptr, "traj"_a=nullptr)
    
    .def("show", (void (VIBesFigTube::*)())&VIBesFigTube::show,
      VIBESFIGTUBE_VOID_SHOW)
    
    .def("show", (void (VIBesFigTube::*)(bool))&VIBesFigTube::show,
      VIBESFIGTUBE_VOID_SHOW_BOOL,
      "detail_slices"_a)
    
    .def("set_cursor", &VIBesFigTube::set_cursor,
      VIBESFIGTUBE_VOID_SET_CURSOR_DOUBLE,
      "t"_a)
    
    .def("show_cursor", &VIBesFigTube::show_cursor,
      VIBESFIGTUBE_VOID_SHOW_CURSOR_BOOL,
      "display"_a=true)

  // Handling tubes

    .def("add_tube", &VIBesFigTube::add_tube,
      VIBESFIGTUBE_VOID_ADD_TUBE_TUBE_STRING_STRING_STRING,
      "tube"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR,
      py::keep_alive<1,2>())
    
    .def("add_tubes", (void (VIBesFigTube::*)(const TubeVector*,const string&,const string&,const string&))&VIBesFigTube::add_tubes,
      VIBESFIGTUBE_VOID_ADD_TUBES_TUBEVECTOR_STRING_STRING_STRING,
      "tubevector"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR,
      py::keep_alive<1,2>())
    
    .def("add_tubes", (void (VIBesFigTube::*)(const TubeVector*,int,int,const string&,const string&,const string&))&VIBesFigTube::add_tubes,
      VIBESFIGTUBE_VOID_ADD_TUBES_TUBEVECTOR_INT_INT_STRING_STRING_STRING,
      "tubevector"_a, "start_index"_a, "end_index"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
   
    .def("set_tube_name", &VIBesFigTube::set_tube_name,
      VIBESFIGTUBE_VOID_SET_TUBE_NAME_TUBE_STRING,
      "tube"_a, "name"_a)
    
    .def("set_tube_derivative", &VIBesFigTube::set_tube_derivative,
      VIBESFIGTUBE_VOID_SET_TUBE_DERIVATIVE_TUBE_TUBE,
      "tube"_a, "derivative"_a)
    
    .def("set_tube_color", (void (VIBesFigTube::*)(const Tube*,const string&,const string&))&VIBesFigTube::set_tube_color,
      VIBESFIGTUBE_VOID_SET_TUBE_COLOR_TUBE_STRING_STRING,
      "tube"_a, "color_frgrnd"_a, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
    
    .def("set_tube_color", (void (VIBesFigTube::*)(const Tube*,TubeColorType,const string&))&VIBesFigTube::set_tube_color,
      VIBESFIGTUBE_VOID_SET_TUBE_COLOR_TUBE_TUBECOLORTYPE_STRING,
      "tube"_a, "color_type"_a, "color"_a)
    
    .def("reset_tube_background", &VIBesFigTube::reset_tube_background,
      VIBESFIGTUBE_VOID_RESET_TUBE_BACKGROUND_TUBE,
      "tube"_a)
    
    .def("remove_tube", &VIBesFigTube::remove_tube,
      VIBESFIGTUBE_VOID_REMOVE_TUBE_TUBE,
      "tube"_a)
  
  // Handling trajectories

    .def("add_trajectory", &VIBesFigTube::add_trajectory,
      VIBESFIGTUBE_VOID_ADD_TRAJECTORY_TRAJECTORY_STRING_STRING,
      "traj"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR,
      py::keep_alive<1,2>())

    .def("add_trajectories", (void (VIBesFigTube::*)(const TrajectoryVector*,const string&,const string&))&VIBesFigTube::add_trajectories,
      VIBESFIGTUBE_VOID_ADD_TRAJECTORIES_TRAJECTORYVECTOR_STRING_STRING,
      "trajvector"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR,
      py::keep_alive<1,2>())

    .def("add_trajectories", (void (VIBesFigTube::*)(const TrajectoryVector*,int,int,const string&,const string&))&VIBesFigTube::add_trajectories,
      VIBESFIGTUBE_VOID_ADD_TRAJECTORIES_TRAJECTORYVECTOR_INT_INT_STRING_STRING,
      "trajvector"_a, "start_index"_a, "end_index"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR)
    
    .def("set_trajectory_name", &VIBesFigTube::set_trajectory_name,
      VIBESFIGTUBE_VOID_SET_TRAJECTORY_NAME_TRAJECTORY_STRING,
      "traj"_a, "name"_a)

    .def("set_trajectory_color", &VIBesFigTube::set_trajectory_color,
      VIBESFIGTUBE_VOID_SET_TRAJECTORY_COLOR_TRAJECTORY_STRING,
      "traj"_a, "color"_a)

    .def("set_trajectory_points_size", &VIBesFigTube::set_trajectory_points_size,
      VIBESFIGTUBE_VOID_SET_TRAJECTORY_POINTS_SIZE_TRAJECTORY_FLOAT,
      "traj"_a, "points_size"_a)

    .def("remove_trajectory", &VIBesFigTube::remove_trajectory,
      VIBESFIGTUBE_VOID_REMOVE_TRAJECTORY_TRAJECTORY,
      "traj"_a)
    ;
}