/** 
 *  Codac binding (graphics)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Paving.h>
#include <codac2_Subpaving.h>
#include <codac2_Figure2D.h>
#include <codac2_Polygon.h>
#include "codac2_py_Figure2D_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_matlab.h"
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Figure2D(py::module& m)
{
  if constexpr(FOR_MATLAB)
  {
    py::class_<GraphicOutput>(m, "GraphicOutput")
      .def(py::init<>())
      .def_static("VIBES", [](){ return GraphicOutput::VIBES; })
      .def_static("IPE", [](){ return GraphicOutput::IPE; })
      .def(py::self | py::self, GRAPHICOUTPUT_OPERATOROR_GRAPHICOUTPUT_GRAPHICOUTPUT)
    ;
  }

  else
  {
    py::enum_<GraphicOutput>(m, "GraphicOutput")
      .value("VIBES", GraphicOutput::VIBES)
      .value("IPE", GraphicOutput::IPE)
      .export_values()
      .def(py::self | py::self, GRAPHICOUTPUT_OPERATOROR_GRAPHICOUTPUT_GRAPHICOUTPUT)
    ;
  }

  py::class_<FigureAxis> exported_axis(m, "FigureAxis", FIGURE2D_MAIN);
  exported_axis

    // disabled because of Matlab convention: .def_readwrite("dim_id", &FigureAxis::dim_id)
    .def_readwrite("limits", &FigureAxis::limits)
    .def_readwrite("label", &FigureAxis::label)
  ;

  m.def("axis", [](Index_type n, const Interval& limits, const std::string& label)
      {
        matlab::test_integer(n);
        return axis(matlab::input_index(n), limits, label);
      },
    FIGUREAXIS_AXIS_INDEX_CONST_INTERVAL_REF_CONST_STRING_REF,
    "dim_id"_a, "limits"_a, "label"_a="");

  py::class_<Figure2D, std::shared_ptr<Figure2D> /* due to enable_shared_from_this */>
    exported(m, "Figure2D", FIGURE2D_MAIN);
  exported
  
    .def(py::init<const std::string&,GraphicOutput,bool>(),
      FIGURE2D_FIGURE2D_CONST_STRING_REF_GRAPHICOUTPUT_BOOL,
      "name"_a, "o"_a, "set_as_default"_a=false)
  
    .def("name", &Figure2D::name,
      CONST_STRING_REF_FIGURE2D_NAME_CONST)
  
    .def("size", &Figure2D::size,
      INDEX_FIGURE2D_SIZE_CONST)
  
    .def("axes", &Figure2D::axes,
      CONST_VECTOR_FIGUREAXIS_REF_FIGURE2D_AXES_CONST)
  
    .def("set_axes", &Figure2D::set_axes,
      FIGURE2D_REF_FIGURE2D_SET_AXES_CONST_FIGUREAXIS_REF_CONST_FIGUREAXIS_REF,
      "axis1"_a, "axis2"_a)
  
    .def("i", &Figure2D::i,
      CONST_INDEX_REF_FIGURE2D_I_CONST)
  
    .def("j", &Figure2D::j,
      CONST_INDEX_REF_FIGURE2D_J_CONST)
  
    .def("pos", &Figure2D::pos,
      CONST_VECTOR_REF_FIGURE2D_POS_CONST)
  
    .def("window_size", &Figure2D::window_size,
      VECTOR_FIGURE2D__WINDOW_SIZE)
  
    .def("set_window_properties", &Figure2D::set_window_properties,
      VOID_FIGURE2D_SET_WINDOW_PROPERTIES_CONST_VECTOR_REF_CONST_VECTOR_REF,
      "pos"_a, "size"_a)
  
    .def("center_viewbox", &Figure2D::center_viewbox,
      VOID_FIGURE2D_CENTER_VIEWBOX_CONST_VECTOR_REF_CONST_VECTOR_REF,
      "c"_a, "r"_a)

    .def("clear", &Figure2D::clear,
      VOID_FIGURE2D_CLEAR)
  
    .def("scaled_unit", &Figure2D::scaled_unit,
      DOUBLE_FIGURE2D_SCALED_UNIT_CONST)
  
    .def("auto_scale", &Figure2D::auto_scale,
      VOID_FIGURE2D_AUTO_SCALE)
  
    .def("is_default", &Figure2D::is_default,
      BOOL_FIGURE2D_IS_DEFAULT_CONST)
  
    .def("set_as_default", &Figure2D::set_as_default,
      VOID_FIGURE2D_SET_AS_DEFAULT)
  
    .def("set_tdomain", &Figure2D::set_tdomain,
      VOID_FIGURE2D_SET_TDOMAIN_CONST_INTERVAL_REF)

    // Geometric shapes

    .def("draw_point", &Figure2D::draw_point,
      VOID_FIGURE2D_DRAW_POINT_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "style"_a=StyleProperties())

    .def("draw_box", &Figure2D::draw_box,
      VOID_FIGURE2D_DRAW_BOX_CONST_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def("draw_circle", &Figure2D::draw_circle,
      VOID_FIGURE2D_DRAW_CIRCLE_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "style"_a=StyleProperties())

    .def("draw_ring", &Figure2D::draw_ring,
      VOID_FIGURE2D_DRAW_RING_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "style"_a=StyleProperties())

    .def("draw_line", (void(Figure2D::*)(const Vector&,const Vector&,const StyleProperties&))&Figure2D::draw_line,
      VOID_FIGURE2D_DRAW_LINE_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "p1"_a, "p2"_a, "style"_a=StyleProperties())

    .def("draw_line", (void(Figure2D::*)(const Segment&,const StyleProperties&))&Figure2D::draw_line,
      VOID_FIGURE2D_DRAW_LINE_CONST_SEGMENT_REF_CONST_STYLEPROPERTIES_REF,
      "e"_a, "style"_a=StyleProperties())

    .def("draw_arrow", &Figure2D::draw_arrow,
      VOID_FIGURE2D_DRAW_ARROW_CONST_VECTOR_REF_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "p1"_a, "p2"_a, "tip_length"_a, "style"_a=StyleProperties())

    .def("draw_polyline", (void(Figure2D::*)(const std::vector<Vector>&,const StyleProperties&))&Figure2D::draw_polyline,
      VOID_FIGURE2D_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def("draw_polyline", (void(Figure2D::*)(const std::vector<Vector>&,float,const StyleProperties&))&Figure2D::draw_polyline,
      VOID_FIGURE2D_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "r"_a, "style"_a=StyleProperties())

    .def("draw_polygon", (void(Figure2D::*)(const Polygon&,const StyleProperties&))&Figure2D::draw_polygon,
      VOID_FIGURE2D_DRAW_POLYGON_CONST_POLYGON_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def("draw_parallelepiped", &Figure2D::draw_parallelepiped,
      VOID_FIGURE2D_DRAW_PARALLELEPIPED_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_STYLEPROPERTIES_REF,
      "z"_a, "A"_a, "style"_a=StyleProperties())

    .def("draw_zonotope", &Figure2D::draw_zonotope,
      VOID_FIGURE2D_DRAW_ZONOTOPE_CONST_VECTOR_REF_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "z"_a, "A"_a, "style"_a=StyleProperties())

    .def("draw_pie", &Figure2D::draw_pie,
      VOID_FIGURE2D_DRAW_PIE_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "theta"_a, "style"_a=StyleProperties())

    .def("draw_ellipse", &Figure2D::draw_ellipse,
      VOID_FIGURE2D_DRAW_ELLIPSE_CONST_VECTOR_REF_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "ab"_a, "theta"_a, "style"_a=StyleProperties())

    .def("draw_ellipsoid", &Figure2D::draw_ellipsoid,
      VOID_FIGURE2D_DRAW_ELLIPSOID_CONST_ELLIPSOID_REF_CONST_STYLEPROPERTIES_REF,
      "e"_a, "style"_a=StyleProperties())

    .def("draw_trajectory", (void(Figure2D::*)(const SampledTraj<Vector>&,const StyleProperties&))&Figure2D::draw_trajectory,
      VOID_FIGURE2D_DRAW_TRAJECTORY_CONST_SAMPLEDTRAJ_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def("draw_trajectory", [](Figure2D& fig, const py::object& x, const StyleProperties& s)
        {
          if(!is_instance<AnalyticTraj<VectorType>>(x)) {
            assert_release("draw_trajectory: invalid function type");
          }

          fig.draw_trajectory(cast<AnalyticTraj<VectorType>>(x), s);
        },
      VOID_FIGURE2D_DRAW_TRAJECTORY_CONST_ANALYTICTRAJ_VECTORTYPE_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def("draw_trajectory", (void(Figure2D::*)(const SampledTraj<Vector>&,const ColorMap&))&Figure2D::draw_trajectory,
      VOID_FIGURE2D_DRAW_TRAJECTORY_CONST_SAMPLEDTRAJ_VECTOR_REF_CONST_COLORMAP_REF,
      "x"_a, "cmap"_a)

    .def("draw_trajectory", [](Figure2D& fig, const py::object& x, const ColorMap& cmap)
        {
          if(!is_instance<AnalyticTraj<VectorType>>(x)) {
            assert_release("draw_trajectory: invalid function type");
          }

          fig.draw_trajectory(cast<AnalyticTraj<VectorType>>(x), cmap);
        },
      VOID_FIGURE2D_DRAW_TRAJECTORY_CONST_ANALYTICTRAJ_VECTORTYPE_REF_CONST_COLORMAP_REF,
      "x"_a, "cmap"_a)

    .def("plot_trajectory", (void(Figure2D::*)(const SampledTraj<double>&,const StyleProperties&))&Figure2D::plot_trajectory,
      VOID_FIGURE2D_PLOT_TRAJECTORY_CONST_SAMPLEDTRAJ_DOUBLE_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def("plot_trajectories", (void(Figure2D::*)(const SampledTraj<Vector>&,const StyleProperties&))&Figure2D::plot_trajectories,
      VOID_FIGURE2D_PLOT_TRAJECTORIES_CONST_SAMPLEDTRAJ_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def("draw_tube", [](Figure2D& fig, const py::object& x, const StyleProperties& s)
        {
          if(!is_instance<SlicedTube<IntervalVector>>(x)) {
            assert_release("draw_tube: invalid function type");
          }

          fig.draw_tube(cast<SlicedTube<IntervalVector>>(x), s);
        },
      VOID_FIGURE2D_DRAW_TUBE_CONST_SLICEDTUBE_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a)

    .def("draw_tube", [](Figure2D& fig, const py::object& x, const ColorMap& cmap)
        {
          if(!is_instance<SlicedTube<IntervalVector>>(x)) {
            assert_release("draw_tube: invalid function type");
          }

          fig.draw_tube(cast<SlicedTube<IntervalVector>>(x), cmap);
        },
      VOID_FIGURE2D_DRAW_TUBE_CONST_SLICEDTUBE_INTERVALVECTOR_REF_CONST_COLORMAP_REF,
      "x"_a, "cmap"_a=ColorMap::blue_tube())

    // Robots

    .def("draw_tank", &Figure2D::draw_tank,
      VOID_FIGURE2D_DRAW_TANK_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "style"_a=StyleProperties())

    .def("draw_AUV", &Figure2D::draw_AUV,
      VOID_FIGURE2D_DRAW_AUV_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "style"_a=StyleProperties())

    .def("draw_motor_boat", &Figure2D::draw_motor_boat,
      VOID_FIGURE2D_DRAW_MOTOR_BOAT_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "style"_a=StyleProperties())

    // Pavings

    .def("draw_paving", (void(Figure2D::*)(const PavingOut&,const StyleProperties&,const StyleProperties&))&Figure2D::draw_paving,
      VOID_FIGURE2D_DRAW_PAVING_CONST_PAVINGOUT_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties::boundary(), "outside_style"_a=StyleProperties::outside())

    .def("draw_paving", (void(Figure2D::*)(const PavingInOut&,const StyleProperties&,const StyleProperties&,const StyleProperties&))&Figure2D::draw_paving,
      VOID_FIGURE2D_DRAW_PAVING_CONST_PAVINGINOUT_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties::boundary(), "outside_style"_a=StyleProperties::outside(), "inside_style"_a=StyleProperties::inside())

    .def("draw_subpaving", (void(Figure2D::*)(const Subpaving<PavingOut>&,const StyleProperties&))&Figure2D::draw_subpaving,
      VOID_FIGURE2D_DRAW_SUBPAVING_CONST_SUBPAVING_P_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "style"_a=StyleProperties())

    .def("draw_subpaving", (void(Figure2D::*)(const Subpaving<PavingInOut>&,const StyleProperties&))&Figure2D::draw_subpaving,
      VOID_FIGURE2D_DRAW_SUBPAVING_CONST_SUBPAVING_P_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "style"_a=StyleProperties())

  ;

  py::class_<DefaultFigure> exported_default_view(m, "DefaultFigure", DEFAULTFIGURE_MAIN);
  exported_default_view

    .def(py::init<>()) // for using static methods in Matlab
  
    .def_static("selected_fig", &DefaultFigure::selected_fig, py::return_value_policy::reference,
      STATIC_SHARED_PTR_FIGURE2D_DEFAULTFIGURE_SELECTED_FIG)
  
    .def_static("set", &DefaultFigure::set,
      STATIC_VOID_DEFAULTFIGURE_SET_SHARED_PTR_FIGURE2D)
  
    .def_static("set_axes", &DefaultFigure::set_axes, py::return_value_policy::reference,
      STATIC_FIGURE2D_REF_DEFAULTFIGURE_SET_AXES_CONST_FIGUREAXIS_REF_CONST_FIGUREAXIS_REF,
      "axis1"_a, "axis2"_a)
  
    .def_static("set_window_properties", &DefaultFigure::set_window_properties,
      STATIC_VOID_DEFAULTFIGURE_SET_WINDOW_PROPERTIES_CONST_VECTOR_REF_CONST_VECTOR_REF)

    .def_static("clear", &DefaultFigure::clear,
      STATIC_VOID_DEFAULTFIGURE_CLEAR)

    // Geometric shapes

    .def_static("draw_point", &DefaultFigure::draw_point,
      STATIC_VOID_DEFAULTFIGURE_DRAW_POINT_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "style"_a=StyleProperties())

    .def_static("draw_box", &DefaultFigure::draw_box,
      STATIC_VOID_DEFAULTFIGURE_DRAW_BOX_CONST_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def_static("draw_circle", &DefaultFigure::draw_circle,
      STATIC_VOID_DEFAULTFIGURE_DRAW_CIRCLE_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "style"_a=StyleProperties())

    .def_static("draw_ring", &DefaultFigure::draw_ring,
      STATIC_VOID_DEFAULTFIGURE_DRAW_RING_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "style"_a=StyleProperties())

    .def_static("draw_line", (void(*)(const Vector&,const Vector&,const StyleProperties&))&DefaultFigure::draw_line,
      STATIC_VOID_DEFAULTFIGURE_DRAW_LINE_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "p1"_a, "p2"_a, "style"_a=StyleProperties())

    .def_static("draw_line", (void(*)(const Segment&,const StyleProperties&))&DefaultFigure::draw_line,
      STATIC_VOID_DEFAULTFIGURE_DRAW_LINE_CONST_SEGMENT_REF_CONST_STYLEPROPERTIES_REF,
      "e"_a, "style"_a=StyleProperties())

    .def_static("draw_arrow", &DefaultFigure::draw_arrow,
      STATIC_VOID_DEFAULTFIGURE_DRAW_ARROW_CONST_VECTOR_REF_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "p1"_a, "p2"_a, "tip_length"_a, "style"_a=StyleProperties())

    .def_static("draw_polyline", (void(*)(const std::vector<Vector>&,const StyleProperties&))&DefaultFigure::draw_polyline,
      STATIC_VOID_DEFAULTFIGURE_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def_static("draw_polyline", (void(*)(const std::vector<Vector>&,float,const StyleProperties&))&DefaultFigure::draw_polyline,
      STATIC_VOID_DEFAULTFIGURE_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "r"_a, "style"_a=StyleProperties())

    .def_static("draw_polygon", (void(*)(const Polygon&,const StyleProperties&))&DefaultFigure::draw_polygon,
      STATIC_VOID_DEFAULTFIGURE_DRAW_POLYGON_CONST_POLYGON_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def_static("draw_parallelepiped", &DefaultFigure::draw_parallelepiped,
      STATIC_VOID_DEFAULTFIGURE_DRAW_PARALLELEPIPED_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_STYLEPROPERTIES_REF,
      "z"_a, "A"_a, "style"_a=StyleProperties())

    .def_static("draw_zonotope", &DefaultFigure::draw_zonotope,
      STATIC_VOID_DEFAULTFIGURE_DRAW_ZONOTOPE_CONST_VECTOR_REF_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "z"_a, "A"_a, "style"_a=StyleProperties())

    .def_static("draw_pie", &DefaultFigure::draw_pie,
      STATIC_VOID_DEFAULTFIGURE_DRAW_PIE_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "theta"_a, "style"_a=StyleProperties())

    .def_static("draw_ellipse", &DefaultFigure::draw_ellipse,
      STATIC_VOID_DEFAULTFIGURE_DRAW_ELLIPSE_CONST_VECTOR_REF_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "ab"_a, "theta"_a, "style"_a=StyleProperties())

    .def_static("draw_ellipsoid", &DefaultFigure::draw_ellipsoid,
      STATIC_VOID_DEFAULTFIGURE_DRAW_ELLIPSOID_CONST_ELLIPSOID_REF_CONST_STYLEPROPERTIES_REF,
      "e"_a, "style"_a=StyleProperties())
    
    .def_static("draw_trajectory", (void(*)(const SampledTraj<Vector>&,const StyleProperties&))&DefaultFigure::draw_trajectory,
      STATIC_VOID_DEFAULTFIGURE_DRAW_TRAJECTORY_CONST_SAMPLEDTRAJ_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def_static("draw_trajectory", [](const py::object& x, const StyleProperties& s)
        {
          if(!is_instance<AnalyticTraj<VectorType>>(x)) {
            assert_release("draw_trajectory: invalid function type");
          }

          DefaultFigure::draw_trajectory(cast<AnalyticTraj<VectorType>>(x), s);
        },
      STATIC_VOID_DEFAULTFIGURE_DRAW_TRAJECTORY_CONST_ANALYTICTRAJ_VECTORTYPE_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def_static("draw_trajectory", (void(*)(const SampledTraj<Vector>&,const ColorMap&))&DefaultFigure::draw_trajectory,
      STATIC_VOID_DEFAULTFIGURE_DRAW_TRAJECTORY_CONST_SAMPLEDTRAJ_VECTOR_REF_CONST_COLORMAP_REF,
      "x"_a, "cmap"_a)

    .def_static("draw_trajectory", [](const py::object& x, const ColorMap& cmap)
        {
          if(!is_instance<AnalyticTraj<VectorType>>(x)) {
            assert_release("draw_trajectory: invalid function type");
          }

          DefaultFigure::draw_trajectory(cast<AnalyticTraj<VectorType>>(x), cmap);
        },
      STATIC_VOID_DEFAULTFIGURE_DRAW_TRAJECTORY_CONST_ANALYTICTRAJ_VECTORTYPE_REF_CONST_COLORMAP_REF,
      "x"_a, "cmap"_a)

    .def_static("plot_trajectory", (void(*)(const SampledTraj<double>&,const StyleProperties&))&DefaultFigure::plot_trajectory,
      STATIC_VOID_DEFAULTFIGURE_PLOT_TRAJECTORY_CONST_SAMPLEDTRAJ_DOUBLE_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def_static("plot_trajectories", (void(*)(const SampledTraj<Vector>&,const StyleProperties&))&DefaultFigure::plot_trajectories,
      STATIC_VOID_DEFAULTFIGURE_PLOT_TRAJECTORIES_CONST_SAMPLEDTRAJ_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a=StyleProperties())

    .def_static("draw_tube", [](const py::object& x, const StyleProperties& s)
        {
          if(!is_instance<SlicedTube<IntervalVector>>(x)) {
            assert_release("draw_tube: invalid function type");
          }

          DefaultFigure::draw_tube(cast<SlicedTube<IntervalVector>>(x), s);
        },
      STATIC_VOID_DEFAULTFIGURE_DRAW_TUBE_CONST_SLICEDTUBE_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "style"_a)

    .def_static("draw_tube", [](const py::object& x, const ColorMap& cmap)
        {
          if(!is_instance<SlicedTube<IntervalVector>>(x)) {
            assert_release("draw_tube: invalid function type");
          }

          DefaultFigure::draw_tube(cast<SlicedTube<IntervalVector>>(x), cmap);
        },
      STATIC_VOID_DEFAULTFIGURE_DRAW_TUBE_CONST_SLICEDTUBE_INTERVALVECTOR_REF_CONST_COLORMAP_REF,
      "x"_a, "cmap"_a)

    // Robots

    .def_static("draw_tank", &DefaultFigure::draw_tank,
      STATIC_VOID_DEFAULTFIGURE_DRAW_TANK_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "style"_a=StyleProperties())

    .def_static("draw_AUV", &DefaultFigure::draw_AUV,
      STATIC_VOID_DEFAULTFIGURE_DRAW_AUV_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "style"_a=StyleProperties())

    .def_static("draw_motor_boat", &DefaultFigure::draw_motor_boat,
      STATIC_VOID_DEFAULTFIGURE_DRAW_MOTOR_BOAT_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "style"_a=StyleProperties())

    // Pavings

    .def_static("draw_paving", (void(*)(const PavingOut&,const StyleProperties&,const StyleProperties&))&DefaultFigure::draw_paving,
      STATIC_VOID_DEFAULTFIGURE_DRAW_PAVING_CONST_PAVINGOUT_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties::boundary(), "outside_style"_a=StyleProperties::outside())

    .def_static("draw_paving", (void(*)(const PavingInOut&,const StyleProperties&,const StyleProperties&,const StyleProperties&))&DefaultFigure::draw_paving,
      STATIC_VOID_DEFAULTFIGURE_DRAW_PAVING_CONST_PAVINGINOUT_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties::boundary(), "outside_style"_a=StyleProperties::outside(), "inside_style"_a=StyleProperties::inside())

    .def_static("draw_subpaving", (void(*)(const Subpaving<PavingOut>&,const StyleProperties&))&DefaultFigure::draw_subpaving,
      STATIC_VOID_DEFAULTFIGURE_DRAW_SUBPAVING_CONST_SUBPAVING_P_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "style"_a=StyleProperties())

    .def_static("draw_subpaving", (void(*)(const Subpaving<PavingInOut>&,const StyleProperties&))&DefaultFigure::draw_subpaving,
      STATIC_VOID_DEFAULTFIGURE_DRAW_SUBPAVING_CONST_SUBPAVING_P_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "style"_a=StyleProperties())

  ;
}
