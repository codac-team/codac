/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Zonotope.h>
#include "codac2_py_Zonotope_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_matlab.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Zonotope(py::module& m)
{
  py::class_<Zonotope> 
    exported(m, "Zonotope", ZONOTOPE_MAIN);
  exported

    .def(py::init<const Vector&,const Matrix&>(),
      ZONOTOPE_ZONOTOPE_CONST_VECTOR_REF_CONST_MATRIX_REF,
      "z"_a, "A"_a)

    .def_static("empty", &Zonotope::empty,
      STATIC_ZONOTOPE_ZONOTOPE_EMPTY_INDEX,
      "n"_a)

    .def("is_empty", &Zonotope::is_empty,
      BOOL_ZONOTOPE_IS_EMPTY_CONST)

    .def("set_empty", &Zonotope::set_empty,
      VOID_ZONOTOPE_SET_EMPTY)

    .def("box", &Zonotope::box,
      INTERVALVECTOR_ZONOTOPE_BOX_CONST)

    .def("proj",[](const Zonotope& x, const std::vector<Index_type>& indices)
        {
          return x.proj(matlab::convert_indices(indices));
        },
      ZONOTOPE_ZONOTOPE_PROJ_CONST_VECTOR_INDEX_REF_CONST,
      "indices"_a)

    .def("__add__", &Zonotope::operator+,
      ZONOTOPE_ZONOTOPE_OPERATORPLUS_CONST_ZONOTOPE_REF,
      py::is_operator())

    .def("__repr__", [](const Zonotope& z) {
          std::ostringstream stream;
          stream << z;
          return string(stream.str()); 
        },
        OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_ZONOTOPE_REF)


    .def_readwrite("c", &Zonotope::c,
      VECTOR_ZONOTOPE_C)

    .def_readwrite("A", &Zonotope::A,
      MATRIX_ZONOTOPE_A)
  
  ;
}
