/** 
 *  Interval Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include <codac2_IntervalMatrix.h>

#include "codac2_py_MatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalMatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_IntervalMatrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_IntervalMatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<IntervalMatrix> export_IntervalMatrix(py::module& m)
{
  py::class_<IntervalMatrix> exported_intervalmatrix_class(m, "IntervalMatrix", INTERVALMATRIX_MAIN);
  export_IntervalMatrixBase<IntervalMatrix,Matrix,false>(exported_intervalmatrix_class);

  exported_intervalmatrix_class

    .def(py::init(
        [](size_t_type r, size_t_type c)
        {
          matlab::test_integer(r,c);
          return std::make_unique<IntervalMatrix>(r,c);
        }),
      INTERVALMATRIX_INTERVALMATRIX_SIZET_SIZET,
      "r"_a, "c"_a)

    .def(py::init(
        [](size_t_type r, size_t_type c, const Interval& x)
        {
          matlab::test_integer(r,c);
          return std::make_unique<IntervalMatrix>(r,c,x);
        }),
      INTERVALMATRIX_INTERVALMATRIX_SIZET_SIZET_CONST_INTERVAL_REF,
      "r"_a, "c"_a, "x"_a)

    .def(py::init(
        [](const std::vector<IntervalVector>& v)
        {
          assert_release(!std::empty(v));
          auto iv = std::make_unique<IntervalMatrix>(v[0].size(),v.size());
          for(size_t i = 0 ; i < v.size() ; i++)
          {
            assert_release(v[i].size() == iv->nb_rows() && "IntervalVector objects of different size");
            iv->col(i) = v[i];
          }
          return iv;
        }),
      INTERVALMATRIX_INTERVALMATRIX_CONST_IV_REF_VARIADIC,
      "v"_a)

    .def(py::init<const IntervalMatrix&>(),
      "x"_a)

    .def(py::init<const Matrix&>(),
      INTERVALMATRIX_INTERVALMATRIX_CONST_MATRIX_REF,
      "x"_a)

    .def(py::init<const Matrix&,const Matrix&>(),
      INTERVALMATRIX_INTERVALMATRIX_CONST_MATRIX_REF_CONST_MATRIX_REF,
      "lb"_a, "ub"_a)

    .def("transpose", &IntervalMatrix::transpose,
      INTERVALMATRIX_INTERVALMATRIX_TRANSPOSE_CONST)

    .def("diag_matrix", &IntervalMatrix::diag_matrix,
      INTERVALMATRIX_INTERVALMATRIX_DIAG_MATRIX_CONST)
    
    .def_static("empty", [](size_t_type r, size_t_type c)
        {
          matlab::test_integer(r,c);
          return IntervalMatrix::empty(r,c);
        },
      STATIC_INTERVALMATRIX_INTERVALMATRIX_EMPTY_SIZET_SIZET,
      "r"_a, "c"_a)

    .def("__repr__", [](const IntervalMatrix& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_INTERVALMATRIX_REF)
  ;

  return exported_intervalmatrix_class;
}

/*
  IntervalMatrix operator*(const Interval& x1, const Matrix& x2);

  IntervalVector operator*(const IntervalMatrix& x1, const Vector& x2);

  template<typename Q_>
  IntervalVector operator*(const MatrixBaseBlock<Q_,Interval>& x1, const Vector& x2);

  IntervalVector operator*(const Matrix& x1, const IntervalVector& x2);

  template<typename Q_>
  IntervalVector operator*(const Matrix& x1, const MatrixBaseBlock<Q_,Interval>& x2);

  template<typename Q1_,typename Q2_,typename T>
  IntervalMatrix operator*(const MatrixBaseBlock<Q1_,Interval>& x1, const MatrixBaseBlock<Q2_,T>& x2)
  {
    return x1.eval() * x2.eval().template cast<Interval>();
  }

  template<typename Q1_,typename Q2_,typename T>
  IntervalMatrix operator*(const MatrixBaseBlock<Q1_,T>& x1, const MatrixBaseBlock<Q2_,Interval>& x2)
  {
    return x1.eval().template cast<Interval>() * x2.eval();
  }

  template<typename Q1_,typename Q2_>
  IntervalMatrix operator*(const MatrixBaseBlock<Q1_,Interval>& x1, const MatrixBaseBlock<Q2_,Interval>& x2)
  {
    return x1.eval() * x2.eval();
  }

  IntervalMatrix operator/(const Matrix& x1, const Interval& x2);
  */