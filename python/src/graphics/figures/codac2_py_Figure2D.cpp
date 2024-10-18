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
#include <codac2_Figure2D.h>
#include "codac2_py_Figure2D_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_matlab.h"
#include <codac2_Paving.h>

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Figure2D(py::module& m)
{
  py::enum_<GraphicOutput>(m, "GraphicOutput")
    .value("VIBES", GraphicOutput::VIBES)
    .value("IPE", GraphicOutput::IPE)
    .def(py::self | py::self, GRAPHICOUTPUT_OPERATOROR_GRAPHICOUTPUT_GRAPHICOUTPUT)
  ;

  py::class_<FigureAxis> exported_axis(m, "FigureAxis", FIGURE2D_MAIN);
  exported_axis

    // disabled because of Matlab convention: .def_readwrite("dim_id", &FigureAxis::dim_id)
    .def_readwrite("limits", &FigureAxis::limits)
    .def_readwrite("label", &FigureAxis::label)
  ;

  m.def("axis", [](size_t_type n, const Interval& limits, const std::string& label)
      {
        matlab::test_integer(n);
        return axis(matlab::input_index(n), limits, label);
      },
    FIGUREAXIS_AXIS_SIZET_CONST_INTERVAL_REF_CONST_STRING_REF,
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
      SIZET_FIGURE2D_SIZE_CONST)
  
    .def("axes", &Figure2D::axes,
      CONST_VECTOR_FIGUREAXIS_REF_FIGURE2D_AXES_CONST)
  
    .def("set_axes", &Figure2D::set_axes,
      VOID_FIGURE2D_SET_AXES_CONST_FIGUREAXIS_REF_CONST_FIGUREAXIS_REF,
      "axis1"_a, "axis2"_a)
  
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
  
    .def("scaled_unit", &Figure2D::scaled_unit,
      DOUBLE_FIGURE2D_SCALED_UNIT_CONST)
  
    .def("is_default", &Figure2D::is_default,
      BOOL_FIGURE2D_IS_DEFAULT_CONST)
  
    .def("set_as_default", &Figure2D::set_as_default,
      VOID_FIGURE2D_SET_AS_DEFAULT)

    // Geometric shapes

    .def("draw_point", &Figure2D::draw_point,
      VOID_FIGURE2D_DRAW_POINT_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "s"_a=StyleProperties())

    .def("draw_box", &Figure2D::draw_box,
      VOID_FIGURE2D_DRAW_BOX_CONST_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def("draw_circle", &Figure2D::draw_circle,
      VOID_FIGURE2D_DRAW_CIRCLE_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "s"_a=StyleProperties())

    .def("draw_ring", &Figure2D::draw_ring,
      VOID_FIGURE2D_DRAW_RING_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "s"_a=StyleProperties())

    .def("draw_polyline", (void(Figure2D::*)(const std::vector<Vector>&,const StyleProperties&))&Figure2D::draw_polyline,
      VOID_FIGURE2D_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def("draw_polyline", (void(Figure2D::*)(const std::vector<Vector>&,float,const StyleProperties&))&Figure2D::draw_polyline,
      VOID_FIGURE2D_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "r"_a, "s"_a=StyleProperties())

    .def("draw_polygone", (void(Figure2D::*)(const std::vector<Vector>&,const StyleProperties&))&Figure2D::draw_polygone,
      VOID_FIGURE2D_DRAW_POLYGONE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def("draw_pie", &Figure2D::draw_pie,
      VOID_FIGURE2D_DRAW_PIE_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "theta"_a, "s"_a=StyleProperties())

    .def("draw_ellipse", &Figure2D::draw_ellipse,
      STATIC_VOID_DEFAULTVIEW_DRAW_ELLIPSE_CONST_VECTOR_REF_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "ab"_a, "theta"_a, "s"_a=StyleProperties())

    // Robots

    .def("draw_tank", &Figure2D::draw_tank,
      VOID_FIGURE2D_DRAW_TANK_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "s"_a=StyleProperties())

    .def("draw_AUV", &Figure2D::draw_AUV,
      VOID_FIGURE2D_DRAW_AUV_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "s"_a=StyleProperties())

  ;

  py::class_<DefaultView> exported_default_view(m, "DefaultView", DEFAULTVIEW_MAIN);
  exported_default_view
  
    .def_static("selected_fig", &DefaultView::selected_fig,
      STATIC_SHARED_PTR_FIGURE2D_DEFAULTVIEW_SELECTED_FIG)
  
    .def_static("set", &DefaultView::set,
      STATIC_VOID_DEFAULTVIEW_SET_SHARED_PTR_FIGURE2D)
  
    .def_static("set_axes", &DefaultView::set_axes,
      STATIC_VOID_DEFAULTVIEW_SET_AXES_CONST_FIGUREAXIS_REF_CONST_FIGUREAXIS_REF)
  
    .def_static("set_window_properties", &DefaultView::set_window_properties,
      STATIC_VOID_DEFAULTVIEW_SET_WINDOW_PROPERTIES_CONST_VECTOR_REF_CONST_VECTOR_REF)

    // Geometric shapes

    .def_static("draw_point", &DefaultView::draw_point,
      STATIC_VOID_DEFAULTVIEW_DRAW_POINT_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "s"_a=StyleProperties())

    .def_static("draw_box", &DefaultView::draw_box,
      STATIC_VOID_DEFAULTVIEW_DRAW_BOX_CONST_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def_static("draw_circle", &DefaultView::draw_circle,
      STATIC_VOID_DEFAULTVIEW_DRAW_CIRCLE_CONST_VECTOR_REF_DOUBLE_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "s"_a=StyleProperties())

    .def_static("draw_ring", &DefaultView::draw_ring,
      STATIC_VOID_DEFAULTVIEW_DRAW_RING_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "s"_a=StyleProperties())

    .def_static("draw_polyline", (void(*)(const std::vector<Vector>&,const StyleProperties&))&DefaultView::draw_polyline,
      STATIC_VOID_DEFAULTVIEW_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def_static("draw_polyline", (void(*)(const std::vector<Vector>&,float,const StyleProperties&))&DefaultView::draw_polyline,
      STATIC_VOID_DEFAULTVIEW_DRAW_POLYLINE_CONST_VECTOR_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "r"_a, "s"_a=StyleProperties())

    .def_static("draw_polygone", (void(*)(const std::vector<Vector>&,const StyleProperties&))&DefaultView::draw_polygone,
      STATIC_VOID_DEFAULTVIEW_DRAW_POLYGONE_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def_static("draw_pie", &DefaultView::draw_pie,
      STATIC_VOID_DEFAULTVIEW_DRAW_PIE_CONST_VECTOR_REF_CONST_INTERVAL_REF_CONST_INTERVAL_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "r"_a, "theta"_a, "s"_a=StyleProperties())

    // Robots

    .def_static("draw_tank", &DefaultView::draw_tank,
      STATIC_VOID_DEFAULTVIEW_DRAW_TANK_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "s"_a=StyleProperties())

    .def_static("draw_AUV", &DefaultView::draw_AUV,
      STATIC_VOID_DEFAULTVIEW_DRAW_AUV_CONST_VECTOR_REF_FLOAT_CONST_STYLEPROPERTIES_REF,
      "x"_a, "size"_a, "s"_a=StyleProperties())

    // Pavings

    .def_static("draw_paving", (void(*)(const PavingOut&,const StyleProperties&,const StyleProperties&))&DefaultView::draw_paving,
      STATIC_VOID_DEFAULTVIEW_DRAW_PAVING_CONST_PAVINGOUT_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties::boundary(), "outside_style"_a=StyleProperties::outside())

    .def_static("draw_paving", (void(*)(const PavingInOut&,const StyleProperties&,const StyleProperties&,const StyleProperties&))&DefaultView::draw_paving,
      STATIC_VOID_DEFAULTVIEW_DRAW_PAVING_CONST_PAVINGINOUT_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties::boundary(), "outside_style"_a=StyleProperties::outside(), "inside_style"_a=StyleProperties::inside())

  ;
}