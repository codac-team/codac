
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
#include "tubex_py_VIBesFigTube_docs.h"
#include "tubex_py_VIBesFigTubeVector_docs.h"
#include "tubex_py_VIBesFigMap_docs.h"
#include "tubex_py_ColorMap_docs.h"

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


//PYBIND11_MODULE(graphics, m){

void export_graphics(py::module& m){
    m.def("beginDrawing", [](){vibes::beginDrawing();});
    m.def("endDrawing", [](){vibes::endDrawing();});
}

void export_VIBesFig(py::module& m){


    py::class_<vibes::Params> params(m, "Params");

    py::class_<VIBesFig>(m, "VIBesFig", "todo")
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

    // py::class_<TubeColorType> tubecolortype(m, "TubeColorType" DOCS_TUBECOLORTYPE);
    py::class_<VIBesFigTube, VIBesFig>(m, "VIBesFigTube", "todo")
      .def(py::init<const std::string &,const Tube *,const Trajectory *>(),
          VIBESFIGTUBE_VIBESFIGTUBE_STRING_TUBE_TRAJECTORY, "fig_name"_a, "tube"_a=nullptr, "traj"_a=nullptr)
      .def("show", (void (VIBesFigTube::*)() )&VIBesFigTube::show,
          VIBESFIGTUBE_VOID_SHOW)
      .def("show", (void (VIBesFigTube::*)(bool) )&VIBesFigTube::show,
          VIBESFIGTUBE_VOID_SHOW_BOOL, "detail_slices"_a)
      .def("set_cursor", &VIBesFigTube::set_cursor,
          VIBESFIGTUBE_VOID_SET_CURSOR_DOUBLE, "t"_a)
      .def("show_cursor", &VIBesFigTube::show_cursor,
          VIBESFIGTUBE_VOID_SHOW_CURSOR_BOOL, "display"_a=true)
      .def("add_tube", &VIBesFigTube::add_tube,
          VIBESFIGTUBE_VOID_ADD_TUBE_TUBE_STRING_STRING_STRING, "tube"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR,
          py::keep_alive<1,2>())
      .def("add_tubes", (void (VIBesFigTube::*)(const TubeVector *,const std::string &,const std::string &,const std::string &) )&VIBesFigTube::add_tubes,
          VIBESFIGTUBE_VOID_ADD_TUBES_TUBEVECTOR_STRING_STRING_STRING, "tubevector"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR,
          py::keep_alive<1,2>())
      .def("add_tubes", (void (VIBesFigTube::*)(const TubeVector *,int,int,const std::string &,const std::string &,const std::string &) )&VIBesFigTube::add_tubes,
          VIBESFIGTUBE_VOID_ADD_TUBES_TUBEVECTOR_INT_INT_STRING_STRING_STRING, "tubevector"_a, "start_index"_a, "end_index"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
      .def("set_tube_name", &VIBesFigTube::set_tube_name,
          VIBESFIGTUBE_VOID_SET_TUBE_NAME_TUBE_STRING, "tube"_a, "name"_a)
      .def("set_tube_derivative", &VIBesFigTube::set_tube_derivative,
          VIBESFIGTUBE_VOID_SET_TUBE_DERIVATIVE_TUBE_TUBE, "tube"_a, "derivative"_a)
      .def("set_tube_color", (void (VIBesFigTube::*)(const Tube *,const std::string &,const std::string &) )&VIBesFigTube::set_tube_color,
          VIBESFIGTUBE_VOID_SET_TUBE_COLOR_TUBE_STRING_STRING, "tube"_a, "color_frgrnd"_a, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
      .def("set_tube_color", (void (VIBesFigTube::*)(const Tube *,TubeColorType,const std::string &) )&VIBesFigTube::set_tube_color,
          VIBESFIGTUBE_VOID_SET_TUBE_COLOR_TUBE_TUBECOLORTYPE_STRING, "tube"_a, "color_type"_a, "color"_a)
      .def("reset_tube_background", &VIBesFigTube::reset_tube_background,
          VIBESFIGTUBE_VOID_RESET_TUBE_BACKGROUND_TUBE, "tube"_a)
      .def("remove_tube", &VIBesFigTube::remove_tube,
          VIBESFIGTUBE_VOID_REMOVE_TUBE_TUBE, "tube"_a)
      .def("add_trajectory", &VIBesFigTube::add_trajectory,
          VIBESFIGTUBE_VOID_ADD_TRAJECTORY_TRAJECTORY_STRING_STRING, "traj"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR,
          py::keep_alive<1,2>())
      .def("add_trajectories", (void (VIBesFigTube::*)(const TrajectoryVector *,const std::string &,const std::string &) )&VIBesFigTube::add_trajectories,
          VIBESFIGTUBE_VOID_ADD_TRAJECTORIES_TRAJECTORYVECTOR_STRING_STRING, "trajvector"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR,
          py::keep_alive<1,2>())
      .def("add_trajectories", (void (VIBesFigTube::*)(const TrajectoryVector *,int,int,const std::string &,const std::string &) )&VIBesFigTube::add_trajectories,
          VIBESFIGTUBE_VOID_ADD_TRAJECTORIES_TRAJECTORYVECTOR_INT_INT_STRING_STRING, "trajvector"_a, "start_index"_a, "end_index"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR)
      .def("set_trajectory_name", &VIBesFigTube::set_trajectory_name,
          VIBESFIGTUBE_VOID_SET_TRAJECTORY_NAME_TRAJECTORY_STRING, "traj"_a, "name"_a)
      .def("set_trajectory_color", &VIBesFigTube::set_trajectory_color,
          VIBESFIGTUBE_VOID_SET_TRAJECTORY_COLOR_TRAJECTORY_STRING, "traj"_a, "color"_a)
      .def("set_trajectory_points_size", &VIBesFigTube::set_trajectory_points_size,
          VIBESFIGTUBE_VOID_SET_TRAJECTORY_POINTS_SIZE_TRAJECTORY_FLOAT, "traj"_a, "points_size"_a)
      .def("remove_trajectory", &VIBesFigTube::remove_trajectory,
          VIBESFIGTUBE_VOID_REMOVE_TRAJECTORY_TRAJECTORY, "traj"_a)
    ;

    py::class_<VIBesFigTubeVector>(m, "VIBesFigTubeVector", "todo")
      .def(py::init<const std::string &>(),
          VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING, "fig_name"_a)
      .def(py::init<const std::string &,int,int>(),
          VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING_INT_INT, "fig_name"_a, "start_index"_a, "end_index"_a)
      .def(py::init<const std::string &,const TubeVector *,const TrajectoryVector *>(),
          VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING_TUBEVECTOR_TRAJECTORYVECTOR, "fig_name"_a, "tubevector"_a, "trajvector"_a=nullptr)
      .def("size", &VIBesFigTubeVector::size,VIBESFIGTUBEVECTOR_INT_SIZE)
      .def("subfigs_number", &VIBesFigTubeVector::subfigs_number,
          VIBESFIGTUBEVECTOR_INT_SUBFIGS_NUMBER)
      .def("set_properties", &VIBesFigTubeVector::set_properties,
          VIBESFIGTUBEVECTOR_VOID_SET_PROPERTIES_INT_INT_INT_INT, "x"_a, "y"_a, "width"_a, "height"_a)
      .def("show", &VIBesFigTubeVector::show,
          VIBESFIGTUBEVECTOR_VOID_SHOW_BOOL, "detail_slices"_a=false)
      .def("set_cursor", &VIBesFigTubeVector::set_cursor,
          VIBESFIGTUBEVECTOR_VOID_SET_CURSOR_DOUBLE, "t"_a)
      .def("show_cursor", &VIBesFigTubeVector::show_cursor,
          VIBESFIGTUBEVECTOR_VOID_SHOW_CURSOR_BOOL, "display"_a=true)
    //   .def("draw_box", (void (VIBesFigTubeVector::*)(const ibex::Interval &,const ibex::IntervalVector &,const vibes::Params &) )&VIBesFigTubeVector::draw_box,
    //       DOCS_VIBESFIGTUBEVECTOR_DRAW_BOX_INTERVAL_INTERVALVECTOR_VIBES::PARAMS, "domain"_a, "box"_a, "params"_a)
    //   .def("draw_box", (void (VIBesFigTubeVector::*)(const ibex::Interval &,const ibex::IntervalVector &,const std::string &,const vibes::Params &) )&VIBesFigTubeVector::draw_box,
    //       DOCS_VIBESFIGTUBEVECTOR_DRAW_BOX_INTERVAL_INTERVALVECTOR_STRING_VIBES::PARAMS, "domain"_a, "box"_a, "color"_a="", "params"_a=vibes::Params())
      .def("add_tube", &VIBesFigTubeVector::add_tube,
          VIBESFIGTUBEVECTOR_VOID_ADD_TUBE_TUBEVECTOR_STRING_STRING_STRING, "tubevector"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
      .def("set_tube_name", &VIBesFigTubeVector::set_tube_name,
          VIBESFIGTUBEVECTOR_VOID_SET_TUBE_NAME_TUBEVECTOR_STRING, "tubevector"_a, "name"_a)
      .def("set_tube_derivative", &VIBesFigTubeVector::set_tube_derivative,
          VIBESFIGTUBEVECTOR_VOID_SET_TUBE_DERIVATIVE_TUBEVECTOR_TUBEVECTOR, "tubevector"_a, "derivative"_a)
      .def("set_tube_color", (void (VIBesFigTubeVector::*)(const TubeVector *,const std::string &,const std::string &) )&VIBesFigTubeVector::set_tube_color,
          VIBESFIGTUBEVECTOR_VOID_SET_TUBE_COLOR_TUBEVECTOR_STRING_STRING, "tubevector"_a, "color_frgrnd"_a, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
      .def("set_tube_color", (void (VIBesFigTubeVector::*)(const TubeVector *,TubeColorType,const std::string &) )&VIBesFigTubeVector::set_tube_color,
          VIBESFIGTUBEVECTOR_VOID_SET_TUBE_COLOR_TUBEVECTOR_TUBECOLORTYPE_STRING, "tubevector"_a, "color_type"_a, "color"_a)
      .def("reset_tube_background", &VIBesFigTubeVector::reset_tube_background,
          VIBESFIGTUBEVECTOR_VOID_RESET_TUBE_BACKGROUND_TUBEVECTOR, "tubevector"_a)
      .def("remove_tube", &VIBesFigTubeVector::remove_tube,
          VIBESFIGTUBEVECTOR_VOID_REMOVE_TUBE_TUBEVECTOR, "tubevector"_a)
      .def("add_trajectory", &VIBesFigTubeVector::add_trajectory,
          VIBESFIGTUBEVECTOR_VOID_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_STRING, "trajvector"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR)
      .def("set_trajectory_name", &VIBesFigTubeVector::set_trajectory_name,
          VIBESFIGTUBEVECTOR_VOID_SET_TRAJECTORY_NAME_TRAJECTORYVECTOR_STRING, "trajvector"_a, "name"_a)
      .def("set_trajectory_color", &VIBesFigTubeVector::set_trajectory_color,
          VIBESFIGTUBEVECTOR_VOID_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_STRING, "trajvector"_a, "color"_a)
      .def("remove_trajectory", &VIBesFigTubeVector::remove_trajectory,
          VIBESFIGTUBEVECTOR_VOID_REMOVE_TRAJECTORY_TRAJECTORYVECTOR, "trajvector"_a)

    // .def("operator[]", [](VIBesFigTubeVector& s,int o) { return s[o];}, 
    //     DOCS_VIBESFIGTUBEVECTOR_OPERATOR_INDEX_INT)
    // .def("operator[]", [](VIBesFigTubeVector& s,int o) { return s[o];}, 
    //     DOCS_VIBESFIGTUBEVECTOR_OPERATOR_INDEX_INT1)
    ;   

    py::class_<VIBesFigMap, VIBesFig>(m, "VIBesFigMap", "todo")
      .def(py::init<const std::string &>(),
          VIBESFIGMAP_VIBESFIGMAP_STRING, "fig_name"_a)
      .def("restrict_tdomain", &VIBesFigMap::restrict_tdomain,
          VIBESFIGMAP_VOID_RESTRICT_TDOMAIN_INTERVAL, "restricted_tdomain"_a)
      .def("enable_tubes_backgrounds", &VIBesFigMap::enable_tubes_backgrounds,
          VIBESFIGMAP_VOID_ENABLE_TUBES_BACKGROUNDS_BOOL, "enable"_a=true)
      .def("show", (void (VIBesFigMap::*)() )&VIBesFigMap::show,
          VIBESFIGMAP_VOID_SHOW)
      .def("show", (void (VIBesFigMap::*)(float) )&VIBesFigMap::show,
          VIBESFIGMAP_VOID_SHOW_FLOAT, "robot_size"_a)
      .def("set_tube_max_disp_slices", &VIBesFigMap::set_tube_max_disp_slices,
          VIBESFIGMAP_VOID_SET_TUBE_MAX_DISP_SLICES_INT, "max"_a)
      .def("set_traj_max_disp_points", &VIBesFigMap::set_traj_max_disp_points,
          VIBESFIGMAP_VOID_SET_TRAJ_MAX_DISP_POINTS_INT, "max"_a)
      .def("smooth_tube_drawing", &VIBesFigMap::smooth_tube_drawing,
          VIBESFIGMAP_VOID_SMOOTH_TUBE_DRAWING_BOOL, "smooth"_a)
      .def("add_tube", &VIBesFigMap::add_tube,
          VIBESFIGMAP_VOID_ADD_TUBE_TUBEVECTOR_STRING_INT_INT, "tube"_a, "name"_a, "index_x"_a, "index_y"_a, 
          py::keep_alive<1,2>())
      .def("set_tube_name", &VIBesFigMap::set_tube_name,
          VIBESFIGMAP_VOID_SET_TUBE_NAME_TUBEVECTOR_STRING, "tube"_a, "name"_a)
      .def("set_tube_color", (void (VIBesFigMap::*)(const TubeVector *,const std::string &) )&VIBesFigMap::set_tube_color,
          VIBESFIGMAP_VOID_SET_TUBE_COLOR_TUBEVECTOR_STRING, "tube"_a, "color"_a)
      .def("set_tube_color", (void (VIBesFigMap::*)(const TubeVector *,const ColorMap &,const Trajectory *) )&VIBesFigMap::set_tube_color,
          VIBESFIGMAP_VOID_SET_TUBE_COLOR_TUBEVECTOR_COLORMAP_TRAJECTORY, "tube"_a, "colormap"_a, "traj_colormap"_a=nullptr)
      .def("remove_tube", &VIBesFigMap::remove_tube,
          VIBESFIGMAP_VOID_REMOVE_TUBE_TUBEVECTOR, "tube"_a)
      .def("add_trajectory", (void (VIBesFigMap::*)(const TrajectoryVector *,const std::string &,int,int,const std::string &) )&VIBesFigMap::add_trajectory,
          VIBESFIGMAP_VOID_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_INT_INT_STRING, "traj"_a, "name"_a, "index_x"_a, "index_y"_a, "color"_a=DEFAULT_TRAJMAP_COLOR)
      .def("add_trajectory", (void (VIBesFigMap::*)(const TrajectoryVector *,const std::string &,int,int,int,const std::string &) )&VIBesFigMap::add_trajectory,
          VIBESFIGMAP_VOID_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_INT_INT_INT_STRING, "traj"_a, "name"_a, "index_x"_a, "index_y"_a, "index_heading"_a, "color"_a=DEFAULT_TRAJMAP_COLOR)
      .def("set_trajectory_name", &VIBesFigMap::set_trajectory_name,
          VIBESFIGMAP_VOID_SET_TRAJECTORY_NAME_TRAJECTORYVECTOR_STRING, "traj"_a, "name"_a)
      .def("set_trajectory_color", (void (VIBesFigMap::*)(const TrajectoryVector *,const std::string &) )&VIBesFigMap::set_trajectory_color,
          VIBESFIGMAP_VOID_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_STRING, "traj"_a, "color"_a)
      .def("set_trajectory_color", (void (VIBesFigMap::*)(const TrajectoryVector *,const ColorMap &,const Trajectory *) )&VIBesFigMap::set_trajectory_color,
          VIBESFIGMAP_VOID_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_COLORMAP_TRAJECTORY, "traj"_a, "colormap"_a, "traj_colormap"_a=nullptr)
      .def("remove_trajectory", &VIBesFigMap::remove_trajectory,
          VIBESFIGMAP_VOID_REMOVE_TRAJECTORY_TRAJECTORYVECTOR, "traj"_a)
       .def("draw_vehicle", (void (VIBesFigMap::*)(const ibex::Vector &,float) )&VIBesFigMap::draw_vehicle,
           VIBESFIGMAP_VOID_DRAW_VEHICLE_VECTOR_FLOAT, "pose"_a, "size"_a=-1)
    //   .def("draw_vehicle", (void (VIBesFigMap::*)(const ibex::Vector &,const vibes::Params &,float) )&VIBesFigMap::draw_vehicle,
    //       DOCS_VIBESFIGMAP_DRAW_VEHICLE_VECTOR_VIBES::PARAMS_FLOAT, "pose"_a, "params"_a, "size"_a=-1)
       .def("draw_vehicle", (void (VIBesFigMap::*)(double,const TrajectoryVector *,float) )&VIBesFigMap::draw_vehicle,
           VIBESFIGMAP_VOID_DRAW_VEHICLE_DOUBLE_TRAJECTORYVECTOR_FLOAT, "t"_a, "traj"_a, "size"_a=-1)
    //   .def("draw_vehicle", (void (VIBesFigMap::*)(double,const TrajectoryVector *,const vibes::Params &,float) )&VIBesFigMap::draw_vehicle,
    //       DOCS_VIBESFIGMAP_DRAW_VEHICLE_DOUBLE_TRAJECTORYVECTOR_VIBES::PARAMS_FLOAT, "t"_a, "traj"_a, "params"_a, "size"_a=-1)
      .def("add_beacon", (void (VIBesFigMap::*)(const ibex::IntervalVector &,const std::string &) )&VIBesFigMap::add_beacon,
          VIBESFIGMAP_VOID_ADD_BEACON_INTERVALVECTOR_STRING, "beacon"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_beacon", (void (VIBesFigMap::*)(const ibex::Vector &,double,const std::string &) )&VIBesFigMap::add_beacon,
          VIBESFIGMAP_VOID_ADD_BEACONS_VECTORBEACON_STRING, "beacon"_a, "width"_a, "color"_a=DEFAULT_BEACON_COLOR)
      //.def("add_beacons", (void (VIBesFigMap::*)(const std::vector<Beacon> &,const std::string &) )&VIBesFigMap::add_beacons,
      //    DOCS_VIBESFIGMAP_ADD_BEACONS_VECTOR_BEACON__STRING, "v_beacons"_a, "color"_a=DEFAULT_BEACON_COLOR)
      //.def("add_beacons", (void (VIBesFigMap::*)(const std::vector<Beacon> &,double,const std::string &) )&VIBesFigMap::add_beacons,
      //    DOCS_VIBESFIGMAP_ADD_BEACONS_VECTOR_BEACON__DOUBLE_STRING, "v_beacons"_a, "width"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_landmarks", (void (VIBesFigMap::*)(const std::vector<ibex::IntervalVector> &,const std::string &) )&VIBesFigMap::add_landmarks,
          VIBESFIGMAP_VOID_ADD_LANDMARKS_VECTORINTERVALVECTOR_STRING, "v_m"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_landmarks", (void (VIBesFigMap::*)(const std::vector<ibex::Vector> &,double,const std::string &) )&VIBesFigMap::add_landmarks,
          VIBESFIGMAP_VOID_ADD_LANDMARKS_VECTORVECTOR_DOUBLE_STRING, "v_m"_a, "width"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_observation", (void (VIBesFigMap::*)(const ibex::IntervalVector &,const ibex::Vector &,const std::string &) )&VIBesFigMap::add_observation,
          VIBESFIGMAP_VOID_ADD_OBSERVATION_INTERVALVECTOR_VECTOR_STRING, "obs"_a, "pose"_a, "color"_a=DEFAULT_OBS_COLOR)
      .def("add_observation", (void (VIBesFigMap::*)(const ibex::IntervalVector &,const TrajectoryVector *,const std::string &) )&VIBesFigMap::add_observation,
          VIBESFIGMAP_VOID_ADD_OBSERVATION_INTERVALVECTOR_TRAJECTORYVECTOR_STRING, "obs"_a, "traj"_a, "color"_a=DEFAULT_OBS_COLOR)
      .def("add_observations", (void (VIBesFigMap::*)(const std::vector<ibex::IntervalVector> &,const ibex::Vector &,const std::string &) )&VIBesFigMap::add_observations,
          VIBESFIGMAP_VOID_ADD_OBSERVATIONS_VECTORINTERVALVECTOR_VECTOR_STRING, "v_obs"_a, "pose"_a, "color"_a=DEFAULT_OBS_COLOR)
      .def("add_observations", (void (VIBesFigMap::*)(const std::vector<ibex::IntervalVector> &,const TrajectoryVector *,const std::string &) )&VIBesFigMap::add_observations,
          VIBESFIGMAP_VOID_ADD_OBSERVATIONS_VECTORINTERVALVECTOR_TRAJECTORYVECTOR_STRING, "v_obs"_a, "traj"_a, "color"_a=DEFAULT_OBS_COLOR)
    ;

    py::class_<ColorMap>(m, "ColorMap")
        .def("is_opaque", &ColorMap::is_opaque, COLORMAP_BOOL_IS_OPAQUE)

    ;
    // bool is_opaque() const;
    // py::class_<FigMapTubeParams> figmaptubeparams(m, "FigMapTubeParams" DOCS_FIGMAPTUBEPARAMS);
    // py::class_<FigMapTrajParams> figmaptrajparams(m, "FigMapTrajParams" DOCS_FIGMAPTRAJPARAMS);

}