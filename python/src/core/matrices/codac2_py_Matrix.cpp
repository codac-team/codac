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
#include <codac2_IntervalMatrix.h>

#include "codac2_py_MatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Matrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalMatrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

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

    .def(py::init(
        [](const std::vector<Vector>& v)
        {
          assert_release(!std::empty(v));
          auto iv = std::make_unique<Matrix>(v.size(),v[0].size());
          for(size_t i = 0 ; i < v.size() ; i++)
          {
            assert_release(v[i].size() == iv->nb_cols() && "Vector objects of different size");
            iv->row(i) = v[i].transpose();
          }
          return iv;
        }),
      MATRIX_MATRIX_INITIALIZER_LIST_INITIALIZER_LIST_DOUBLE,
      "v"_a)

    .def("transpose", &Matrix::transpose,
      MATRIX_MATRIX_TRANSPOSE_CONST)

    .def("diag_matrix", &Matrix::diag_matrix,
      MATRIX_MATRIX_DIAG_MATRIX_CONST)

    .def("inverse", &Matrix::inverse,
      MATRIX_MATRIX_INVERSE_CONST)

  ;

  // Matrix operations

  exported_matrix_class

    //IntervalMatrix operator*(const Interval& x1, const Matrix& x2);
    .def("__rmul__", [](const Matrix& x2, const Interval& x1) { return x1*x2; }, py::is_operator(),
      INTERVALMATRIX_OPERATORMUL_CONST_INTERVAL_REF_CONST_MATRIX_REF)

    //IntervalVector operator*(const Matrix& x1, const IntervalVector& x2);
    .def("__mul__", [](const Matrix& x1, const IntervalVector& x2) { return x1*x2; }, py::is_operator(),
      INTERVALVECTOR_OPERATORMUL_CONST_MATRIX_REF_CONST_INTERVALVECTOR_REF)

    //IntervalVector operator*(const Matrix& x1, const MatrixBaseBlock<Q_,Interval>& x2);
    .def("__mul__", [](const Matrix& x1, const MatrixBaseBlock<EigenMatrix<Interval>&,Interval>& x2) { return x1*x2; }, py::is_operator(),
      "todo")

    //IntervalMatrix operator/(const Matrix& x1, const Interval& x2);
    .def("__truediv__", [](const Matrix& x1, const Interval& x2) { return x1/x2; }, py::is_operator(),
      INTERVALMATRIX_OPERATORDIV_CONST_MATRIX_REF_CONST_INTERVAL_REF)

  ;
}