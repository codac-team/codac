/** 
 *  \file
 *  VIBesFigMap Python binding
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

#include "tubex_VIBesFigMap.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_VIBesFigMap_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_VIBesFigMap(py::module& m)
{
  py::class_<VIBesFigMap,VIBesFig> fig_map(m, "VIBesFigMap", VIBESFIGMAP_MAIN);
  fig_map

  // Basics

    .def(py::init<const string&>(),
      VIBESFIGMAP_VIBESFIGMAP_STRING,
      "fig_name"_a)

    .def("restrict_tdomain", &VIBesFigMap::restrict_tdomain,
      VIBESFIGMAP_VOID_RESTRICT_TDOMAIN_INTERVAL,
      "restricted_tdomain"_a)

    .def("enable_tubes_backgrounds", &VIBesFigMap::enable_tubes_backgrounds,
      VIBESFIGMAP_VOID_ENABLE_TUBES_BACKGROUNDS_BOOL,
      "enable"_a=true)

    .def("show", (void (VIBesFigMap::*)())&VIBesFigMap::show,
      VIBESFIGMAP_VOID_SHOW)

    .def("show", (void (VIBesFigMap::*)(float))&VIBesFigMap::show,
      VIBESFIGMAP_VOID_SHOW_FLOAT,
      "robot_size"_a)

    .def("set_tube_max_disp_slices", &VIBesFigMap::set_tube_max_disp_slices,
      VIBESFIGMAP_VOID_SET_TUBE_MAX_DISP_SLICES_INT,
      "max"_a)

    .def("set_traj_max_disp_points", &VIBesFigMap::set_traj_max_disp_points,
      VIBESFIGMAP_VOID_SET_TRAJ_MAX_DISP_POINTS_INT,
      "max"_a)

    .def("smooth_tube_drawing", &VIBesFigMap::smooth_tube_drawing,
      VIBESFIGMAP_VOID_SMOOTH_TUBE_DRAWING_BOOL,
      "smooth"_a)

  // Handling tubes

    .def("add_tube", &VIBesFigMap::add_tube,
      VIBESFIGMAP_VOID_ADD_TUBE_TUBEVECTOR_STRING_INT_INT,
      "tube"_a, "name"_a, "index_x"_a, "index_y"_a, 
      py::keep_alive<1,2>())

    .def("set_tube_name", &VIBesFigMap::set_tube_name,
      VIBESFIGMAP_VOID_SET_TUBE_NAME_TUBEVECTOR_STRING,
      "tube"_a, "name"_a)

    .def("set_tube_color", (void (VIBesFigMap::*)(const TubeVector*,const string&))&VIBesFigMap::set_tube_color,
      VIBESFIGMAP_VOID_SET_TUBE_COLOR_TUBEVECTOR_STRING,
      "tube"_a, "color"_a)

    .def("set_tube_color", (void (VIBesFigMap::*)(const TubeVector*,const ColorMap&,const Trajectory*))&VIBesFigMap::set_tube_color,
      VIBESFIGMAP_VOID_SET_TUBE_COLOR_TUBEVECTOR_COLORMAP_TRAJECTORY,
      "tube"_a, "colormap"_a, "traj_colormap"_a=nullptr)

    .def("remove_tube", &VIBesFigMap::remove_tube,
      VIBESFIGMAP_VOID_REMOVE_TUBE_TUBEVECTOR,
      "tube"_a)

  // Handling trajectories

    .def("add_trajectory", (void (VIBesFigMap::*)(const TrajectoryVector*,const string&,int,int,const string&))&VIBesFigMap::add_trajectory,
      VIBESFIGMAP_VOID_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_INT_INT_STRING,
      "traj"_a, "name"_a, "index_x"_a, "index_y"_a, "color"_a=DEFAULT_TRAJMAP_COLOR)
    
    .def("add_trajectory", (void (VIBesFigMap::*)(const TrajectoryVector*,const string&,int,int,int,const string&))&VIBesFigMap::add_trajectory,
      VIBESFIGMAP_VOID_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_INT_INT_INT_STRING,
      "traj"_a, "name"_a, "index_x"_a, "index_y"_a, "index_heading"_a, "color"_a=DEFAULT_TRAJMAP_COLOR)
    
    .def("set_trajectory_name", &VIBesFigMap::set_trajectory_name,
      VIBESFIGMAP_VOID_SET_TRAJECTORY_NAME_TRAJECTORYVECTOR_STRING,
      "traj"_a, "name"_a)
    
    .def("set_trajectory_color", (void (VIBesFigMap::*)(const TrajectoryVector*,const string&))&VIBesFigMap::set_trajectory_color,
      VIBESFIGMAP_VOID_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_STRING,
      "traj"_a, "color"_a)
    
    .def("set_trajectory_color", (void (VIBesFigMap::*)(const TrajectoryVector*,const ColorMap&,const Trajectory*))&VIBesFigMap::set_trajectory_color,
      VIBESFIGMAP_VOID_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_COLORMAP_TRAJECTORY,
      "traj"_a, "colormap"_a, "traj_colormap"_a=nullptr)
    
    .def("remove_trajectory", &VIBesFigMap::remove_trajectory,
      VIBESFIGMAP_VOID_REMOVE_TRAJECTORY_TRAJECTORYVECTOR,
      "traj"_a)

  // Handling robotics objects

    .def("draw_vehicle", (void (VIBesFigMap::*)(const Vector&,float))&VIBesFigMap::draw_vehicle,
      VIBESFIGMAP_VOID_DRAW_VEHICLE_VECTOR_FLOAT,
      "pose"_a, "size"_a=-1)

    .def("draw_vehicle", (void (VIBesFigMap::*)(double,const TrajectoryVector*,float))&VIBesFigMap::draw_vehicle,
      VIBESFIGMAP_VOID_DRAW_VEHICLE_DOUBLE_TRAJECTORYVECTOR_FLOAT,
      "t"_a, "traj"_a, "size"_a=-1)

    .def("add_beacon", (void (VIBesFigMap::*)(const IntervalVector&,const string&))&VIBesFigMap::add_beacon,
      VIBESFIGMAP_VOID_ADD_BEACON_INTERVALVECTOR_STRING,
      "beacon"_a, "color"_a=DEFAULT_BEACON_COLOR)
    
    .def("add_beacon", (void (VIBesFigMap::*)(const Vector&,double,const string&))&VIBesFigMap::add_beacon,
      VIBESFIGMAP_VOID_ADD_BEACONS_VECTORBEACON_STRING,
      "beacon"_a, "width"_a, "color"_a=DEFAULT_BEACON_COLOR)

    .def("add_landmarks", (void (VIBesFigMap::*)(const vector<IntervalVector>&,const string&))&VIBesFigMap::add_landmarks,
      VIBESFIGMAP_VOID_ADD_LANDMARKS_VECTORINTERVALVECTOR_STRING,
      "v_m"_a, "color"_a=DEFAULT_BEACON_COLOR)
    
    .def("add_landmarks", (void (VIBesFigMap::*)(const vector<Vector>&,double,const string&))&VIBesFigMap::add_landmarks,
      VIBESFIGMAP_VOID_ADD_LANDMARKS_VECTORVECTOR_DOUBLE_STRING,
      "v_m"_a, "width"_a, "color"_a=DEFAULT_BEACON_COLOR)
    
    .def("add_observation", (void (VIBesFigMap::*)(const IntervalVector&,const Vector&,const string&))&VIBesFigMap::add_observation,
      VIBESFIGMAP_VOID_ADD_OBSERVATION_INTERVALVECTOR_VECTOR_STRING,
      "obs"_a, "pose"_a, "color"_a=DEFAULT_OBS_COLOR)
    
    .def("add_observation", (void (VIBesFigMap::*)(const IntervalVector&,const TrajectoryVector*,const string&))&VIBesFigMap::add_observation,
      VIBESFIGMAP_VOID_ADD_OBSERVATION_INTERVALVECTOR_TRAJECTORYVECTOR_STRING,
      "obs"_a, "traj"_a, "color"_a=DEFAULT_OBS_COLOR)
    
    .def("add_observations", (void (VIBesFigMap::*)(const vector<IntervalVector>&,const Vector&,const string&))&VIBesFigMap::add_observations,
      VIBESFIGMAP_VOID_ADD_OBSERVATIONS_VECTORINTERVALVECTOR_VECTOR_STRING,
      "v_obs"_a, "pose"_a, "color"_a=DEFAULT_OBS_COLOR)
    
    .def("add_observations", (void (VIBesFigMap::*)(const vector<IntervalVector>&,const TrajectoryVector*,const string&))&VIBesFigMap::add_observations,
      VIBESFIGMAP_VOID_ADD_OBSERVATIONS_VECTORINTERVALVECTOR_TRAJECTORYVECTOR_STRING,
      "v_obs"_a, "traj"_a, "color"_a=DEFAULT_OBS_COLOR)
  ;
}