/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include <codac2_Figure.h>
#include "codac2_py_Figure_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Figure(py::module& m)
{
  py::enum_<GraphicOutput>(m, "GraphicOutput")
    .value("VIBES", GraphicOutput::VIBES)
    .value("IPE", GraphicOutput::IPE)
    .def(py::self | py::self, GRAPHICOUTPUT_OPERATOROR_GRAPHICOUTPUT_GRAPHICOUTPUT)
  ;

  py::class_<FigureAxis> exported_axis(m, "FigureAxis", FIGURE_MAIN);
  exported_axis

    // disabled because of Matlab convention: .def_readwrite("dim_id", &FigureAxis::dim_id)
    .def_readwrite("limits", &FigureAxis::limits)
    .def_readwrite("label", &FigureAxis::label)
  ;

  m.def("axis", [](size_t_type n, const Interval& limits, const std::string& label)
      {
        matlab::test_integer(n);
        matlab::scale_index(n);
        return axis(n,limits,label);
      },
    FIGUREAXIS_AXIS_SIZET_CONST_INTERVAL_REF_CONST_STRING_REF,
    "dim_id"_a, "limits"_a, "label"_a="");

  py::class_<Figure> exported(m, "Figure", FIGURE_MAIN);
  exported
  
    .def(py::init<const std::string&,GraphicOutput,bool>(),
      FIGURE_FIGURE_CONST_STRING_REF_GRAPHICOUTPUT_BOOL,
      "name"_a, "o"_a, "set_as_default"_a=false)
  
    .def("name", &Figure::name,
      CONST_STRING_REF_FIGURE_NAME_CONST)
  
    .def("size", &Figure::size,
      SIZET_FIGURE_SIZE_CONST)
  
    .def("axes", &Figure::axes,
      CONST_VECTOR_FIGUREAXIS_REF_FIGURE_AXES_CONST)
  
    .def("set_axes", &Figure::set_axes,
      VOID_FIGURE_SET_AXES_CONST_FIGUREAXIS_REF_CONST_FIGUREAXIS_REF,
      "axis1"_a, "axis2"_a)
  
    .def("pos", &Figure::pos,
      CONST_VECTOR_REF_FIGURE_POS_CONST)
  
    .def("window_size", &Figure::window_size,
      VECTOR_FIGURE__WINDOW_SIZE)
  
    .def("set_window_properties", &Figure::set_window_properties,
      VOID_FIGURE_SET_WINDOW_PROPERTIES_CONST_VECTOR_REF_CONST_VECTOR_REF,
      "pos"_a, "size"_a)
  
    .def("center_viewbox", &Figure::center_viewbox,
      VOID_FIGURE_CENTER_VIEWBOX_CONST_VECTOR_REF_CONST_VECTOR_REF,
      "c"_a, "r"_a)
  
    .def("scaled_unit", &Figure::scaled_unit,
      DOUBLE_FIGURE_SCALED_UNIT_CONST)
  
    .def("is_default", &Figure::is_default,
      BOOL_FIGURE_IS_DEFAULT_CONST)
  
    .def("set_as_default", &Figure::set_as_default,
      VOID_FIGURE_SET_AS_DEFAULT)

    // Geometric shapes

    .def("draw_point", &Figure::draw_point,
      VOID_FIGURE_DRAW_POINT_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "s"_a=StyleProperties())

    .def("draw_box", &Figure::draw_box,
      VOID_FIGURE_DRAW_BOX_CONST_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def("draw_circle", &Figure::draw_circle,
      VOID_FIGURE_DRAW_CIRCLE_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "s"_a=StyleProperties())

    .def("draw_ring", &Figure::draw_ring,
      VOID_FIGURE_DRAW_RING_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "s"_a=StyleProperties())

    .def("draw_polyline", (void(Figure::*)(const std::vector<Vector>&,const StyleProperties&))&Figure::draw_polyline,
      VOID_FIGURE_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def("draw_polyline", (void(Figure::*)(const std::vector<Vector>&,float,const StyleProperties&))&Figure::draw_polyline,
      VOID_FIGURE_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "r"_a, "s"_a=StyleProperties())

    .def("draw_polygone", (void(Figure::*)(const std::vector<Vector>&,const StyleProperties&))&Figure::draw_polygone,
      VOID_FIGURE_DRAW_POLYGONE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def("draw_pie", &Figure::draw_pie,
      VOID_FIGURE_DRAW_PIE_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "theta"_a, "s"_a=StyleProperties())

    // Robots

    .def("draw_tank", &Figure::draw_tank,
      VOID_FIGURE_DRAW_TANK_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "s"_a=StyleProperties())

    .def("draw_AUV", &Figure::draw_AUV,
      VOID_FIGURE_DRAW_AUV_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "s"_a=StyleProperties())

  ;

  py::class_<DefaultFigure> exported_default_figure(m, "DefaultFigure", DEFAULTFIGURE_MAIN);
  exported_default_figure
  
    .def("selected_fig", &DefaultFigure::selected_fig,
      STATIC_FIGURE__PTR_DEFAULTFIGURE_SELECTED_FIG)
  
    .def("set", &DefaultFigure::set,
      STATIC_VOID_DEFAULTFIGURE_SET_FIGURE__PTR)
  
    .def("set_axes", &DefaultFigure::set_axes,
      STATIC_VOID_DEFAULTFIGURE_SET_AXES_CONST_FIGUREAXIS_REF_CONST_FIGUREAXIS_REF)
  
    .def("set_window_properties", &DefaultFigure::set_window_properties,
      STATIC_VOID_DEFAULTFIGURE_SET_WINDOW_PROPERTIES_CONST_VECTOR_REF_CONST_VECTOR_REF)

    // Geometric shapes

    .def_static("draw_point", &DefaultFigure::draw_point,
      STATIC_VOID_DEFAULTFIGURE_DRAW_POINT_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "s"_a=StyleProperties())

    .def_static("draw_box", &DefaultFigure::draw_box,
      STATIC_VOID_DEFAULTFIGURE_DRAW_BOX_CONST_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def_static("draw_circle", &DefaultFigure::draw_circle,
      STATIC_VOID_DEFAULTFIGURE_DRAW_CIRCLE_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "s"_a=StyleProperties())

    .def_static("draw_ring", &DefaultFigure::draw_ring,
      STATIC_VOID_DEFAULTFIGURE_DRAW_RING_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "s"_a=StyleProperties())

    .def_static("draw_polyline", (void(*)(const std::vector<Vector>&,const StyleProperties&))&DefaultFigure::draw_polyline,
      STATIC_VOID_DEFAULTFIGURE_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def_static("draw_polyline", (void(*)(const std::vector<Vector>&,float,const StyleProperties&))&DefaultFigure::draw_polyline,
      STATIC_VOID_DEFAULTFIGURE_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "r"_a, "s"_a=StyleProperties())

    .def_static("draw_polygone", (void(*)(const std::vector<Vector>&,const StyleProperties&))&DefaultFigure::draw_polygone,
      STATIC_VOID_DEFAULTFIGURE_DRAW_POLYGONE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def_static("draw_pie", &DefaultFigure::draw_pie,
      STATIC_VOID_DEFAULTFIGURE_DRAW_PIE_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "theta"_a, "s"_a=StyleProperties())

    // Robots

    .def_static("draw_tank", &DefaultFigure::draw_tank,
      STATIC_VOID_DEFAULTFIGURE_DRAW_TANK_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "s"_a=StyleProperties())

    .def_static("draw_AUV", &DefaultFigure::draw_AUV,
      STATIC_VOID_DEFAULTFIGURE_DRAW_AUV_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "s"_a=StyleProperties())

  ;
}