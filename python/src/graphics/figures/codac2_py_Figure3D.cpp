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

    .def("draw_box", &Figure3D::draw_box,
      VOID_FIGURE3D_DRAW_BOX_CONST_INTERVALVECTOR_REF_CONST_STYLEPROPERTIES_REF,
      "x"_a, "s"_a=StyleProperties())

    .def("draw_parallelepiped", &Figure3D::draw_parallelepiped,
      VOID_FIGURE3D_DRAW_PARALLELEPIPED_CONST_VECTOR_REF_CONST_MATRIX_REF_CONST_STYLEPROPERTIES_REF,
      "z"_a, "A"_a, "s"_a=StyleProperties())


    // Pavings

    .def("draw_paving", (void(Figure3D::*)(const PavingOut&, const StyleProperties&))&Figure3D::draw_paving,
      VOID_FIGURE3D_DRAW_PAVING_CONST_PAVINGOUT_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties(Color::yellow(0.5)))

    .def("draw_paving", (void(Figure3D::*)(const PavingInOut&,const StyleProperties&, const StyleProperties&))&Figure3D::draw_paving,
      VOID_FIGURE3D_DRAW_PAVING_CONST_PAVINGINOUT_REF_CONST_STYLEPROPERTIES_REF_CONST_STYLEPROPERTIES_REF,
      "p"_a, "boundary_style"_a=StyleProperties(Color::yellow(0.15)), "inside_style"_a=StyleProperties(Color::green(0.5)))

  ;
}