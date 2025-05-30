/** 
 *  Codac binding (graphics)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <codac2_Paving.h>
#include <codac2_Figure3D.h>
#include "codac2_py_Figure3D_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_matlab.h"
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Figure3D(py::module& m)
{
  py::class_<Figure3D>
    exported(m, "Figure3D", FIGURE3D_MAIN);
  exported
  
    .def(py::init<const std::string&>(),
      FIGURE3D_FIGURE3D_CONST_STRING_REF,
      "name"_a)
  
    .def("name", &Figure3D::name,
      CONST_STRING_REF_FIGURE3D_NAME_CONST)

    .def("draw_axes", &Figure3D::draw_axes,
      VOID_FIGURE3D_DRAW_AXES_DOUBLE,
      "size"_a=1.0)

    // Geometric shapes
    .def("draw_triangle", (void(Figure3D::*)(const Vector &c, const Matrix &A, const Vector &p1, const Vector &p2, const Vector &p3, const StyleProperties &s))&Figure3D::draw_triangle,
       VOID_FIGURE3D_DRAW_TRIANGLE_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "A"_a, "p1"_a, "p2"_a, "p3"_a, "s"_a=StyleProperties())

    .def("draw_triangle", (void(Figure3D::*)(const Vector &p1, const Vector &p2, const Vector &p3, const StyleProperties &s))&Figure3D::draw_triangle,
       VOID_FIGURE3D_DRAW_TRIANGLE_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "p1"_a, "p2"_a, "p3"_a, "s"_a=StyleProperties())

    .def("draw_polygon", &Figure3D::draw_polygon,
       VOID_FIGURE3D_DRAW_POLYGON_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "A"_a, "l"_a, "s"_a=StyleProperties())

    .def("draw_parallelogram", &Figure3D::draw_parallelogram,
       VOID_FIGURE3D_DRAW_PARALLELOGRAM_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "A"_a, "p"_a, "v1"_a, "v2"_a, "s"_a=StyleProperties())

    .def("draw_box", &Figure3D::draw_box,
      VOID_FIGURE3D_DRAW_BOX_CONST_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def("draw_parallelepiped", &Figure3D::draw_parallelepiped,
      VOID_FIGURE3D_DRAW_PARALLELEPIPED_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_STYLEPROPERTIES_REF,
      "z"_a, "A"_a, "s"_a=StyleProperties())

    .def("draw_zonotope", &Figure3D::draw_zonotope,
      VOID_FIGURE3D_DRAW_ZONOTOPE_CONST_VECTOR_REF_CONST_VECTOR_VECTOR_REF_CONST_STYLEPROPERTIES_REF, 
      "z"_a, "A"_a, "s"_a=StyleProperties())

    .def("draw_arrow", &Figure3D::draw_arrow,
      VOID_FIGURE3D_DRAW_ARROW_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "A"_a, "s"_a=StyleProperties())

    .def("draw_surface", &Figure3D::draw_surface,
      VOID_FIGURE3D_DRAW_SURFACE_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_INTERVAL_REF_DOUBLE_CONST_INTERVAL_REF_DOUBLE_FUNCTION_VECTOR_DOUBLEDOUBLE__CONST_STYLEPROPERTIES_REF,
      "c"_a, "A"_a, "Ip1"_a, "dp1"_a, "Ip2"_a, "dp2"_a, "f"_a, "s"_a=StyleProperties())

    .def("draw_sphere", &Figure3D::draw_sphere,
      VOID_FIGURE3D_DRAW_SPHERE_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "A"_a, "s"_a=StyleProperties(Color::blue(0.5)))
    
    .def("draw_ellipsoid", &Figure3D::draw_ellipsoid,
       VOID_FIGURE3D_DRAW_ELLIPSOID_CONST_ELLIPSOID_REF_CONST_STYLEPROPERTIES_REF,
      "e"_a, "s"_a=StyleProperties(Color::blue(0.5)))

    .def("draw_car", &Figure3D::draw_car,
      VOID_FIGURE3D_DRAW_CAR_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_STYLEPROPERTIES_REF,
      "c"_a, "A"_a, "s"_a=StyleProperties(Color::yellow(0.5)))
    
    .def("draw_plane", &Figure3D::draw_plane,
      VOID_FIGURE3D_DRAW_PLANE_CONST_VECTOR_REF_CONST_MATRIX_REF_BOOL_CONST_STYLEPROPERTIES_REF,
      "c"_a, "A"_a, "yaw_is_up"_a=true, "s"_a=StyleProperties(Color::dark_gray(0.8)))
    

    // Pavings

    .def("draw_paving", (void(Figure3D::*)(const PavingOut&, const StyleProperties&))&Figure3D::draw_paving,
      VOID_FIGURE3D_DRAW_PAVING_CONST_PAVINGOUT_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties(Color::yellow(0.5)))

    .def("draw_paving", (void(Figure3D::*)(const PavingInOut&,const StyleProperties&, const StyleProperties&))&Figure3D::draw_paving,
      VOID_FIGURE3D_DRAW_PAVING_CONST_PAVINGINOUT_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties(Color::yellow(0.15), "boundary"), "inside_style"_a=StyleProperties(Color::green(0.5), "inside"))

  ;
}
