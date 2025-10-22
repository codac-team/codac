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
#include "codac2_py_cast.h"

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

    .def("proj",[](const Zonotope& x, const std::vector<Index_type>& indices)
        {
          return x.proj(matlab::convert_indices(indices));
        },
      ZONOTOPE_ZONOTOPE_PROJ_CONST_VECTOR_INDEX_REF_CONST,
      "indices"_a)

    .def_readwrite("z", &Zonotope::z,
      VECTOR_ZONOTOPE_Z)

    .def_readwrite("A", &Zonotope::A,
      MATRIX_ZONOTOPE_A)
  
  ;
}
