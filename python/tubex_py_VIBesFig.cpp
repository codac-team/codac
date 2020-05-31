
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
// #include "tubex_VIBesFigMap.h"
// #include <tubex-rob.h>
#include "tubex_py_VIBesFig_docs.h"
#include "tubex_py_VIBesFigTube_docs.h"
#include "tubex_py_VIBesFigTubeVector_docs.h"
#include "tubex_py_VIBesFigMap_docs.h"


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

    py::class_<VIBesFig>(m, "VIBesFig", DOCS_VIBESFIG)
      .def(py::init<const std::string &>(),
          DOCS_VIBESFIG_VIBESFIG_STRING, "fig_name"_a)
      .def("close", &VIBesFig::close,DOCS_VIBESFIG_CLOSE)
      .def("set_properties", &VIBesFig::set_properties,
          DOCS_VIBESFIG_SET_PROPERTIES_INT_INT_INT_INT, "x"_a, "y"_a, "width"_a, "height"_a)
      .def("set_background", &VIBesFig::set_background,
          DOCS_VIBESFIG_SET_BACKGROUND_STRING, "bg_color"_a)
      .def("axis_limits", (const ibex::IntervalVector & (VIBesFig::*)(double,double,double,double,bool,float) )&VIBesFig::axis_limits,
          DOCS_VIBESFIG_AXIS_LIMITS_DOUBLE_DOUBLE_DOUBLE_DOUBLE_BOOL_FLOAT, "x_min"_a, "x_max"_a, "y_min"_a, "y_max"_a, "same_ratio"_a=false, "margin"_a=0)
      .def("axis_limits", (const ibex::IntervalVector & (VIBesFig::*)(const ibex::IntervalVector &,bool,float) )&VIBesFig::axis_limits,
          DOCS_VIBESFIG_AXIS_LIMITS_INTERVALVECTOR_BOOL_FLOAT, "viewbox"_a, "same_ratio"_a=false, "margin"_a=0)
      .def("save_image", &VIBesFig::save_image,
          DOCS_VIBESFIG_SAVE_IMAGE_STRING_STRING_STRING, "suffix"_a="", "extension"_a="svg", "path"_a=".")
      .def("show", &VIBesFig::show,DOCS_VIBESFIG_SHOW)
      .def("clear", &VIBesFig::clear,DOCS_VIBESFIG_CLEAR)
      .def("draw_box", (void (VIBesFig::*)(const ibex::IntervalVector &,const std::string &,const vibes::Params &))&VIBesFig::draw_box,
           "todo", "box"_a, "color"_a = "black", "params"_a=vibes::Params())
      .def("draw_circle", (void (VIBesFig::*)(double,double,double,const std::string &,const vibes::Params &))&VIBesFig::draw_circle,
           "todo", "x"_a, "y"_a, "r"_a, "color"_a = "black", "params"_a=vibes::Params())
      .def("draw_ring", (void (VIBesFig::*)(double,double,const ibex::Interval&,const std::string &,const vibes::Params &))&VIBesFig::draw_ring,
           "todo", "x"_a, "y"_a, "r"_a, "color"_a = "black", "params"_a=vibes::Params())
      .def("draw_pie", (void (VIBesFig::*)(double,double,const ibex::Interval &,const ibex::Interval &,const std::string &,const vibes::Params &))&VIBesFig::draw_pie,
           "todo", "x"_a, "y"_a, "r"_a, "theta"_a, "color"_a = "black", "params"_a=vibes::Params())
    //   .def("draw_box", (void (VIBesFigTubeVector::*)(const ibex::Interval &,const ibex::IntervalVector &,const std::string &,const vibes::Params &) )&VIBesFigTubeVector::draw_box,
    //       DOCS_VIBESFIGTUBEVECTOR_DRAW_BOX_INTERVAL_INTERVALVECTOR_STRING_VIBES::PARAMS, "domain"_a, "box"_a, "color"_a="", "params"_a=vibes::Params())
    ;

    // py::class_<TubeColorType> tubecolortype(m, "TubeColorType" DOCS_TUBECOLORTYPE);
    py::class_<VIBesFigTube, VIBesFig>(m, "VIBesFigTube", DOCS_VIBESFIGTUBE)
      .def(py::init<const std::string &,const Tube *,const Trajectory *>(),
          DOCS_VIBESFIGTUBE_VIBESFIGTUBE_STRING_TUBE_TRAJECTORY, "fig_name"_a, "tube"_a=nullptr, "traj"_a=nullptr)
      .def("show", (void (VIBesFigTube::*)() )&VIBesFigTube::show,
          DOCS_VIBESFIGTUBE_SHOW)
      .def("show", (void (VIBesFigTube::*)(bool) )&VIBesFigTube::show,
          DOCS_VIBESFIGTUBE_SHOW_BOOL, "detail_slices"_a)
      .def("set_cursor", &VIBesFigTube::set_cursor,
          DOCS_VIBESFIGTUBE_SET_CURSOR_DOUBLE, "t"_a)
      .def("show_cursor", &VIBesFigTube::show_cursor,
          DOCS_VIBESFIGTUBE_SHOW_CURSOR_BOOL, "display"_a=true)
      .def("add_tube", &VIBesFigTube::add_tube,
          DOCS_VIBESFIGTUBE_ADD_TUBE_TUBE_STRING_STRING_STRING, "tube"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR,
          py::keep_alive<1,2>())
      .def("add_tubes", (void (VIBesFigTube::*)(const TubeVector *,const std::string &,const std::string &,const std::string &) )&VIBesFigTube::add_tubes,
          DOCS_VIBESFIGTUBE_ADD_TUBES_TUBEVECTOR_STRING_STRING_STRING, "tubevector"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR,
          py::keep_alive<1,2>())
      .def("add_tubes", (void (VIBesFigTube::*)(const TubeVector *,int,int,const std::string &,const std::string &,const std::string &) )&VIBesFigTube::add_tubes,
          DOCS_VIBESFIGTUBE_ADD_TUBES_TUBEVECTOR_INT_INT_STRING_STRING_STRING, "tubevector"_a, "start_index"_a, "end_index"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
      .def("set_tube_name", &VIBesFigTube::set_tube_name,
          DOCS_VIBESFIGTUBE_SET_TUBE_NAME_TUBE_STRING, "tube"_a, "name"_a)
      .def("set_tube_derivative", &VIBesFigTube::set_tube_derivative,
          DOCS_VIBESFIGTUBE_SET_TUBE_DERIVATIVE_TUBE_TUBE, "tube"_a, "derivative"_a)
      .def("set_tube_color", (void (VIBesFigTube::*)(const Tube *,const std::string &,const std::string &) )&VIBesFigTube::set_tube_color,
          DOCS_VIBESFIGTUBE_SET_TUBE_COLOR_TUBE_STRING_STRING, "tube"_a, "color_frgrnd"_a, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
      .def("set_tube_color", (void (VIBesFigTube::*)(const Tube *,TubeColorType,const std::string &) )&VIBesFigTube::set_tube_color,
          DOCS_VIBESFIGTUBE_SET_TUBE_COLOR_TUBE_TUBECOLORTYPE_STRING, "tube"_a, "color_type"_a, "color"_a)
      .def("reset_tube_background", &VIBesFigTube::reset_tube_background,
          DOCS_VIBESFIGTUBE_RESET_TUBE_BACKGROUND_TUBE, "tube"_a)
      .def("remove_tube", &VIBesFigTube::remove_tube,
          DOCS_VIBESFIGTUBE_REMOVE_TUBE_TUBE, "tube"_a)
      .def("add_trajectory", &VIBesFigTube::add_trajectory,
          DOCS_VIBESFIGTUBE_ADD_TRAJECTORY_TRAJECTORY_STRING_STRING, "traj"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR,
          py::keep_alive<1,2>())
      .def("add_trajectories", (void (VIBesFigTube::*)(const TrajectoryVector *,const std::string &,const std::string &) )&VIBesFigTube::add_trajectories,
          DOCS_VIBESFIGTUBE_ADD_TRAJECTORIES_TRAJECTORYVECTOR_STRING_STRING, "trajvector"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR,
          py::keep_alive<1,2>())
      .def("add_trajectories", (void (VIBesFigTube::*)(const TrajectoryVector *,int,int,const std::string &,const std::string &) )&VIBesFigTube::add_trajectories,
          DOCS_VIBESFIGTUBE_ADD_TRAJECTORIES_TRAJECTORYVECTOR_INT_INT_STRING_STRING, "trajvector"_a, "start_index"_a, "end_index"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR)
      .def("set_trajectory_name", &VIBesFigTube::set_trajectory_name,
          DOCS_VIBESFIGTUBE_SET_TRAJECTORY_NAME_TRAJECTORY_STRING, "traj"_a, "name"_a)
      .def("set_trajectory_color", &VIBesFigTube::set_trajectory_color,
          DOCS_VIBESFIGTUBE_SET_TRAJECTORY_COLOR_TRAJECTORY_STRING, "traj"_a, "color"_a)
      .def("set_trajectory_points_size", &VIBesFigTube::set_trajectory_points_size,
          DOCS_VIBESFIGTUBE_SET_TRAJECTORY_POINTS_SIZE_TRAJECTORY_FLOAT, "traj"_a, "points_size"_a)
      .def("remove_trajectory", &VIBesFigTube::remove_trajectory,
          DOCS_VIBESFIGTUBE_REMOVE_TRAJECTORY_TRAJECTORY, "traj"_a)
    ;

    py::class_<VIBesFigTubeVector>(m, "VIBesFigTubeVector", DOCS_VIBESFIGTUBEVECTOR)
      .def(py::init<const std::string &>(),
          DOCS_VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING, "fig_name"_a)
      .def(py::init<const std::string &,int,int>(),
          DOCS_VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING_INT_INT, "fig_name"_a, "start_index"_a, "end_index"_a)
      .def(py::init<const std::string &,const TubeVector *,const TrajectoryVector *>(),
          DOCS_VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING_TUBEVECTOR_TRAJECTORYVECTOR, "fig_name"_a, "tubevector"_a, "trajvector"_a=nullptr)
      .def("size", &VIBesFigTubeVector::size,DOCS_VIBESFIGTUBEVECTOR_SIZE)
      .def("subfigs_number", &VIBesFigTubeVector::subfigs_number,
          DOCS_VIBESFIGTUBEVECTOR_SUBFIGS_NUMBER)
      .def("set_properties", &VIBesFigTubeVector::set_properties,
          DOCS_VIBESFIGTUBEVECTOR_SET_PROPERTIES_INT_INT_INT_INT, "x"_a, "y"_a, "width"_a, "height"_a)
      .def("show", &VIBesFigTubeVector::show,
          DOCS_VIBESFIGTUBEVECTOR_SHOW_BOOL, "detail_slices"_a=false)
      .def("set_cursor", &VIBesFigTubeVector::set_cursor,
          DOCS_VIBESFIGTUBEVECTOR_SET_CURSOR_DOUBLE, "t"_a)
      .def("show_cursor", &VIBesFigTubeVector::show_cursor,
          DOCS_VIBESFIGTUBEVECTOR_SHOW_CURSOR_BOOL, "display"_a=true)
    //   .def("draw_box", (void (VIBesFigTubeVector::*)(const ibex::Interval &,const ibex::IntervalVector &,const vibes::Params &) )&VIBesFigTubeVector::draw_box,
    //       DOCS_VIBESFIGTUBEVECTOR_DRAW_BOX_INTERVAL_INTERVALVECTOR_VIBES::PARAMS, "domain"_a, "box"_a, "params"_a)
    //   .def("draw_box", (void (VIBesFigTubeVector::*)(const ibex::Interval &,const ibex::IntervalVector &,const std::string &,const vibes::Params &) )&VIBesFigTubeVector::draw_box,
    //       DOCS_VIBESFIGTUBEVECTOR_DRAW_BOX_INTERVAL_INTERVALVECTOR_STRING_VIBES::PARAMS, "domain"_a, "box"_a, "color"_a="", "params"_a=vibes::Params())
      .def("add_tube", &VIBesFigTubeVector::add_tube,
          DOCS_VIBESFIGTUBEVECTOR_ADD_TUBE_TUBEVECTOR_STRING_STRING_STRING, "tubevector"_a, "name"_a, "color_frgrnd"_a=DEFAULT_FRGRND_COLOR, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
      .def("set_tube_name", &VIBesFigTubeVector::set_tube_name,
          DOCS_VIBESFIGTUBEVECTOR_SET_TUBE_NAME_TUBEVECTOR_STRING, "tubevector"_a, "name"_a)
      .def("set_tube_derivative", &VIBesFigTubeVector::set_tube_derivative,
          DOCS_VIBESFIGTUBEVECTOR_SET_TUBE_DERIVATIVE_TUBEVECTOR_TUBEVECTOR, "tubevector"_a, "derivative"_a)
      .def("set_tube_color", (void (VIBesFigTubeVector::*)(const TubeVector *,const std::string &,const std::string &) )&VIBesFigTubeVector::set_tube_color,
          DOCS_VIBESFIGTUBEVECTOR_SET_TUBE_COLOR_TUBEVECTOR_STRING_STRING, "tubevector"_a, "color_frgrnd"_a, "color_bckgrnd"_a=DEFAULT_BCKGRND_COLOR)
      .def("set_tube_color", (void (VIBesFigTubeVector::*)(const TubeVector *,TubeColorType,const std::string &) )&VIBesFigTubeVector::set_tube_color,
          DOCS_VIBESFIGTUBEVECTOR_SET_TUBE_COLOR_TUBEVECTOR_TUBECOLORTYPE_STRING, "tubevector"_a, "color_type"_a, "color"_a)
      .def("reset_tube_background", &VIBesFigTubeVector::reset_tube_background,
          DOCS_VIBESFIGTUBEVECTOR_RESET_TUBE_BACKGROUND_TUBEVECTOR, "tubevector"_a)
      .def("remove_tube", &VIBesFigTubeVector::remove_tube,
          DOCS_VIBESFIGTUBEVECTOR_REMOVE_TUBE_TUBEVECTOR, "tubevector"_a)
      .def("add_trajectory", &VIBesFigTubeVector::add_trajectory,
          DOCS_VIBESFIGTUBEVECTOR_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_STRING, "trajvector"_a, "name"_a, "color"_a=DEFAULT_TRAJ_COLOR)
      .def("set_trajectory_name", &VIBesFigTubeVector::set_trajectory_name,
          DOCS_VIBESFIGTUBEVECTOR_SET_TRAJECTORY_NAME_TRAJECTORYVECTOR_STRING, "trajvector"_a, "name"_a)
      .def("set_trajectory_color", &VIBesFigTubeVector::set_trajectory_color,
          DOCS_VIBESFIGTUBEVECTOR_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_STRING, "trajvector"_a, "color"_a)
      .def("remove_trajectory", &VIBesFigTubeVector::remove_trajectory,
          DOCS_VIBESFIGTUBEVECTOR_REMOVE_TRAJECTORY_TRAJECTORYVECTOR, "trajvector"_a)

    // .def("operator[]", [](VIBesFigTubeVector& s,int o) { return s[o];}, 
    //     DOCS_VIBESFIGTUBEVECTOR_OPERATOR_INDEX_INT)
    // .def("operator[]", [](VIBesFigTubeVector& s,int o) { return s[o];}, 
    //     DOCS_VIBESFIGTUBEVECTOR_OPERATOR_INDEX_INT1)
    ;   

    py::class_<VIBesFigMap, VIBesFig>(m, "VIBesFigMap", DOCS_VIBESFIGMAP)
      .def(py::init<const std::string &>(),
          DOCS_VIBESFIGMAP_VIBESFIGMAP_STRING, "fig_name"_a)
      .def("restrict_tdomain", &VIBesFigMap::restrict_tdomain,
          DOCS_VIBESFIGMAP_SET_RESTRICTED_TDOMAIN_INTERVAL, "restricted_tdomain"_a)
      .def("enable_tubes_backgrounds", &VIBesFigMap::enable_tubes_backgrounds,
          DOCS_VIBESFIGMAP_ENABLE_TUBES_BACKGROUNDS_BOOL, "enable"_a=true)
      .def("show", (void (VIBesFigMap::*)() )&VIBesFigMap::show,
          DOCS_VIBESFIGMAP_SHOW)
      .def("show", (void (VIBesFigMap::*)(float) )&VIBesFigMap::show,
          DOCS_VIBESFIGMAP_SHOW_FLOAT, "robot_size"_a)
      .def("set_tube_max_disp_slices", &VIBesFigMap::set_tube_max_disp_slices,
          DOCS_VIBESFIGMAP_SET_TUBE_MAX_DISP_SLICES_INT, "max"_a)
      .def("set_traj_max_disp_points", &VIBesFigMap::set_traj_max_disp_points,
          DOCS_VIBESFIGMAP_SET_TRAJ_MAX_DISP_POINTS_INT, "max"_a)
      .def("smooth_tube_drawing", &VIBesFigMap::smooth_tube_drawing,
          DOCS_VIBESFIGMAP_SMOOTH_TUBE_DRAWING_BOOL, "smooth"_a)
      .def("add_tube", &VIBesFigMap::add_tube,
          DOCS_VIBESFIGMAP_ADD_TUBE_TUBEVECTOR_STRING_INT_INT, "tube"_a, "name"_a, "index_x"_a, "index_y"_a, 
          py::keep_alive<1,2>())
      .def("set_tube_name", &VIBesFigMap::set_tube_name,
          DOCS_VIBESFIGMAP_SET_TUBE_NAME_TUBEVECTOR_STRING, "tube"_a, "name"_a)
      .def("set_tube_color", (void (VIBesFigMap::*)(const TubeVector *,const std::string &) )&VIBesFigMap::set_tube_color,
          DOCS_VIBESFIGMAP_SET_TUBE_COLOR_TUBEVECTOR_STRING, "tube"_a, "color"_a)
      .def("set_tube_color", (void (VIBesFigMap::*)(const TubeVector *,const ColorMap &,const Trajectory *) )&VIBesFigMap::set_tube_color,
          DOCS_VIBESFIGMAP_SET_TUBE_COLOR_TUBEVECTOR_COLORMAP_TRAJECTORY, "tube"_a, "colormap"_a, "traj_colormap"_a=nullptr)
      .def("remove_tube", &VIBesFigMap::remove_tube,
          DOCS_VIBESFIGMAP_REMOVE_TUBE_TUBEVECTOR, "tube"_a)
      .def("add_trajectory", (void (VIBesFigMap::*)(const TrajectoryVector *,const std::string &,int,int,const std::string &) )&VIBesFigMap::add_trajectory,
          DOCS_VIBESFIGMAP_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_INT_INT_STRING, "traj"_a, "name"_a, "index_x"_a, "index_y"_a, "color"_a=DEFAULT_TRAJMAP_COLOR)
      .def("add_trajectory", (void (VIBesFigMap::*)(const TrajectoryVector *,const std::string &,int,int,int,const std::string &) )&VIBesFigMap::add_trajectory,
          DOCS_VIBESFIGMAP_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_INT_INT_INT_STRING, "traj"_a, "name"_a, "index_x"_a, "index_y"_a, "index_heading"_a, "color"_a=DEFAULT_TRAJMAP_COLOR)
      .def("set_trajectory_name", &VIBesFigMap::set_trajectory_name,
          DOCS_VIBESFIGMAP_SET_TRAJECTORY_NAME_TRAJECTORYVECTOR_STRING, "traj"_a, "name"_a)
      .def("set_trajectory_color", (void (VIBesFigMap::*)(const TrajectoryVector *,const std::string &) )&VIBesFigMap::set_trajectory_color,
          DOCS_VIBESFIGMAP_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_STRING, "traj"_a, "color"_a)
      .def("set_trajectory_color", (void (VIBesFigMap::*)(const TrajectoryVector *,const ColorMap &,const Trajectory *) )&VIBesFigMap::set_trajectory_color,
          DOCS_VIBESFIGMAP_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_COLORMAP_TRAJECTORY, "traj"_a, "colormap"_a, "traj_colormap"_a=nullptr)
      .def("remove_trajectory", &VIBesFigMap::remove_trajectory,
          DOCS_VIBESFIGMAP_REMOVE_TRAJECTORY_TRAJECTORYVECTOR, "traj"_a)
       .def("draw_vehicle", (void (VIBesFigMap::*)(const ibex::Vector &,float) )&VIBesFigMap::draw_vehicle,
           DOCS_VIBESFIGMAP_DRAW_VEHICLE_VECTOR_FLOAT, "pose"_a, "size"_a=-1)
    //   .def("draw_vehicle", (void (VIBesFigMap::*)(const ibex::Vector &,const vibes::Params &,float) )&VIBesFigMap::draw_vehicle,
    //       DOCS_VIBESFIGMAP_DRAW_VEHICLE_VECTOR_VIBES::PARAMS_FLOAT, "pose"_a, "params"_a, "size"_a=-1)
       .def("draw_vehicle", (void (VIBesFigMap::*)(double,const TrajectoryVector *,float) )&VIBesFigMap::draw_vehicle,
           DOCS_VIBESFIGMAP_DRAW_VEHICLE_DOUBLE_TRAJECTORYVECTOR_FLOAT, "t"_a, "traj"_a, "size"_a=-1)
    //   .def("draw_vehicle", (void (VIBesFigMap::*)(double,const TrajectoryVector *,const vibes::Params &,float) )&VIBesFigMap::draw_vehicle,
    //       DOCS_VIBESFIGMAP_DRAW_VEHICLE_DOUBLE_TRAJECTORYVECTOR_VIBES::PARAMS_FLOAT, "t"_a, "traj"_a, "params"_a, "size"_a=-1)
      .def("add_beacon", (void (VIBesFigMap::*)(const ibex::IntervalVector &,const std::string &) )&VIBesFigMap::add_beacon,
          DOCS_VIBESFIGMAP_ADD_BEACON_BEACON_STRING, "beacon"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_beacon", (void (VIBesFigMap::*)(const ibex::Vector &,double,const std::string &) )&VIBesFigMap::add_beacon,
          DOCS_VIBESFIGMAP_ADD_BEACON_BEACON_DOUBLE_STRING, "beacon"_a, "width"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_beacons", (void (VIBesFigMap::*)(const std::vector<Beacon> &,const std::string &) )&VIBesFigMap::add_beacons,
          DOCS_VIBESFIGMAP_ADD_BEACONS_VECTOR_BEACON__STRING, "v_beacons"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_beacons", (void (VIBesFigMap::*)(const std::vector<Beacon> &,double,const std::string &) )&VIBesFigMap::add_beacons,
          DOCS_VIBESFIGMAP_ADD_BEACONS_VECTOR_BEACON__DOUBLE_STRING, "v_beacons"_a, "width"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_landmarks", (void (VIBesFigMap::*)(const std::vector<ibex::IntervalVector> &,const std::string &) )&VIBesFigMap::add_landmarks,
          DOCS_VIBESFIGMAP_ADD_LANDMARKS_VECTOR_INTERVALVECTOR__STRING, "v_m"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_landmarks", (void (VIBesFigMap::*)(const std::vector<ibex::Vector> &,double,const std::string &) )&VIBesFigMap::add_landmarks,
          DOCS_VIBESFIGMAP_ADD_LANDMARKS_VECTOR_VECTOR__DOUBLE_STRING, "v_m"_a, "width"_a, "color"_a=DEFAULT_BEACON_COLOR)
      .def("add_observation", (void (VIBesFigMap::*)(const ibex::IntervalVector &,const ibex::Vector &,const std::string &) )&VIBesFigMap::add_observation,
          DOCS_VIBESFIGMAP_ADD_OBSERVATION_INTERVALVECTOR_VECTOR_STRING, "obs"_a, "pose"_a, "color"_a=DEFAULT_OBS_COLOR)
      .def("add_observation", (void (VIBesFigMap::*)(const ibex::IntervalVector &,const TrajectoryVector *,const std::string &) )&VIBesFigMap::add_observation,
          DOCS_VIBESFIGMAP_ADD_OBSERVATION_INTERVALVECTOR_TRAJECTORYVECTOR_STRING, "obs"_a, "traj"_a, "color"_a=DEFAULT_OBS_COLOR)
      .def("add_observations", (void (VIBesFigMap::*)(const std::vector<ibex::IntervalVector> &,const ibex::Vector &,const std::string &) )&VIBesFigMap::add_observations,
          DOCS_VIBESFIGMAP_ADD_OBSERVATIONS_VECTOR_INTERVALVECTOR__VECTOR_STRING, "v_obs"_a, "pose"_a, "color"_a=DEFAULT_OBS_COLOR)
      .def("add_observations", (void (VIBesFigMap::*)(const std::vector<ibex::IntervalVector> &,const TrajectoryVector *,const std::string &) )&VIBesFigMap::add_observations,
          DOCS_VIBESFIGMAP_ADD_OBSERVATIONS_VECTOR_INTERVALVECTOR__TRAJECTORYVECTOR_STRING, "v_obs"_a, "traj"_a, "color"_a=DEFAULT_OBS_COLOR)
    ;

    py::class_<ColorMap>(m, "ColorMap")
        .def("is_opaque", &ColorMap::is_opaque)

    ;
    // bool is_opaque() const;
    // py::class_<FigMapTubeParams> figmaptubeparams(m, "FigMapTubeParams" DOCS_FIGMAPTUBEPARAMS);
    // py::class_<FigMapTrajParams> figmaptrajparams(m, "FigMapTrajParams" DOCS_FIGMAPTRAJPARAMS);

}