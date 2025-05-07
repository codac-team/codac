/** 
 *  Codac binding (unsupported)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Parallelepiped.h>
#include "codac2_py_Parallelepiped_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_matlab.h"
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Parallelepiped(py::module& m)
{
  py::class_<Parallelepiped> 
    exported(m, "Parallelepiped", PARALLELEPIPED_MAIN);
  exported

  .def(py::init<const Vector&, const Matrix&>(),
    PARALLELEPIPED_PARALLELEPIPED_CONST_VECTOR_REF_CONST_MATRIX_REF,
    "z"_a, "A"_a)

  .def("vertices", &Parallelepiped::vertices,
    VECTOR_VECTOR_PARALLELEPIPED_VERTICES)

  .def_readwrite("z", &Parallelepiped::z)

  .def_readwrite("A", &Parallelepiped::A)
  ;
}

