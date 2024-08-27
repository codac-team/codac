/** 
 *  Matrix binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include <codac2_Matrix.h>

#include "codac2_py_MatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Matrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_MatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Matrix(py::module& m)
{
  py::class_<Matrix> exported_matrix_class(m, "Matrix", MATRIX_MAIN);
  export_MatrixBase<Matrix,double,false>(m, exported_matrix_class);

  exported_matrix_class

    .def(py::init(
        [](size_t_type r, size_t_type c)
        {
          matlab::test_integer(r,c);
          return std::make_unique<Matrix>(r,c);
        }),
      MATRIX_MATRIX_SIZET_SIZET,
      "r"_a, "c"_a)

    .def(py::init(
        [](size_t_type r, size_t_type c, double x)
        {
          matlab::test_integer(r,c);
          return std::make_unique<Matrix>(r,c,x);
        }),
      MATRIX_MATRIX_SIZET_SIZET_DOUBLE,
      "r"_a, "c"_a, "x"_a)

    .def("transpose", &Matrix::transpose,
      MATRIX_MATRIX_TRANSPOSE_CONST)

    .def("diag_matrix", &Matrix::diag_matrix,
      MATRIX_MATRIX_DIAG_MATRIX_CONST)

    .def("inverse", &Matrix::inverse,
      MATRIX_MATRIX_INVERSE_CONST)

  ;
}