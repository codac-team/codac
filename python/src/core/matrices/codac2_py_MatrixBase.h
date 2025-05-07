/** 
 *  MatrixBase binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Matrix.h>
#include "codac2_py_matlab.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename S,typename T,bool VECTOR_INHERITANCE>
void export_MatrixBase(py::module& m, py::class_<S>& pyclass)
{
  pyclass

    .def(py::self == py::self)
    .def(py::self != py::self)

    .def("__eq__", [](const S& x1, const Eigen::Matrix<T,-1,-1>& x2)
        {
          return x1 == x2;
        },
      DOC_TO_BE_DEFINED)

    .def("__len__", [](const S& x)
        {
          return x.size();
        },
      DOC_TO_BE_DEFINED)

    .def("size", [](const S& x)
        {
          return x.size();
        },
      DOC_TO_BE_DEFINED)

    .def("rows", [](const S& x)
        {
          return x.rows();
        },
      DOC_TO_BE_DEFINED)

    .def("cols", [](const S& x)
        {
          return x.cols();
        },
      DOC_TO_BE_DEFINED)

    .def("min_coeff", [](const S& x)
        {
          return x.min_coeff();
        },
      MATRIXBASE_ADDONS_BASE_SCALAR_MIN_COEFF_CONST)

    .def("max_coeff", [](const S& x)
        {
          return x.max_coeff();
        },
      MATRIXBASE_ADDONS_BASE_SCALAR_MAX_COEFF_CONST)

    .def("norm", [](const S& x)
        {
          return x.norm();
        },
      DOC_TO_BE_DEFINED)

    .def("squared_norm", [](const S& x)
        {
          return x.squared_norm();
        },
      MATRIXBASE_ADDONS_BASE_AUTO_SQUARED_NORM_CONST)

  ;

  if constexpr(!VECTOR_INHERITANCE)
  {
    pyclass

    .def("is_squared", [](const S& x)
        {
          return x.is_squared();
        },
      MATRIXBASE_ADDONS_BASE_BOOL_IS_SQUARED_CONST)

    .def("__getitem__", [](const S& x, const py::tuple& ij) -> const T&
        {
          if constexpr(FOR_MATLAB)
            assert_release(py::isinstance<py::int_>(ij[0]) && py::isinstance<py::int_>(ij[1]));

          int i = ij[0].cast<int>();
          int j = ij[1].cast<int>();

          return x(matlab::input_index(i), matlab::input_index(j));
        }, py::return_value_policy::reference_internal,
      MATRIX_ADDONS_BASE_CONST_SCALAR_REF_OPERATORCALL_INDEX_INDEX_CONST)

    .def("__setitem__", [](S& x, const py::tuple& ij, const T& a)
        {
          if constexpr(FOR_MATLAB)
            assert_release(py::isinstance<py::int_>(ij[0]) && py::isinstance<py::int_>(ij[1]));

          int i = ij[0].cast<int>();
          int j = ij[1].cast<int>();

          x(matlab::input_index(i), matlab::input_index(j)) = a;
        },
      MATRIX_ADDONS_BASE_SCALAR_REF_OPERATORCALL_INDEX_INDEX)

    ;
  }

  pyclass

    .def("init", [](S& x, const T& a)
        {
          x.init(a);
        },
      MATRIX_ADDONS_BASE_AUTO_REF_INIT_CONST_SCALAR_REF,
      "x"_a)

    .def("init", [](S& x, const S& a)
        {
          x.init(a);
        },
      MATRIX_ADDONS_BASE_AUTO_REF_INIT_CONST_MATRIX_SCALARROWSATCOMPILETIMECOLSATCOMPILETIME_REF,
      "x"_a)

    .def("__repr__", [](const S& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      DOC_TO_BE_DEFINED)

    .def("transpose", [](const S& x) -> Eigen::Matrix<T,-1,-1>
        {
          return x.transpose().eval();
        },
      DOC_TO_BE_DEFINED)

    .def("diag_matrix", [](const S& x)
        {
          return x.diagonal().asDiagonal();
        },
      DOC_TO_BE_DEFINED)
  ;

  if constexpr(!VECTOR_INHERITANCE)
  {
    // Virtual inheritance of VectorBase would shaddow the following methods.
    // Therefore they are not defined in case of this inheritance.

    pyclass

      .def("block", [](S& x, Index_type i, Index_type j, Index_type p, Index_type q) -> Eigen::Matrix<T,-1,-1>
          {
            matlab::test_integer(i,j);
            matlab::test_integer(p,q);
            return x.block(matlab::input_index(i),matlab::input_index(j),matlab::input_index(p),matlab::input_index(q));
          },
        py::keep_alive<0,1>(),
        DOC_TO_BE_DEFINED)

      .def("col", [](S& x, Index_type i) -> Eigen::Matrix<T,-1,1>
          {
            matlab::test_integer(i);
            return x.col(matlab::input_index(i)).eval();
          },
        DOC_TO_BE_DEFINED)

      .def("row", [](S& x, Index_type i) -> Eigen::Matrix<T,1,-1>
          {
            matlab::test_integer(i);
            return x.row(matlab::input_index(i)).eval();
          },
        DOC_TO_BE_DEFINED)

      .def("set_block", [](S& x, Index_type i, Index_type j, Index_type p, Index_type q, const S& y)
          {
            matlab::test_integer(i,j);
            matlab::test_integer(p,q);
            x.block(matlab::input_index(i),matlab::input_index(j),matlab::input_index(p),matlab::input_index(q)) = y;
          },
        py::keep_alive<0,1>(),
        DOC_TO_BE_DEFINED)

      .def("set_block", [](S& x, Index_type i, Index_type j, Index_type p, Index_type q, const Eigen::Matrix<T,-1,1>& y)
          {
            matlab::test_integer(i,j);
            matlab::test_integer(p,q);
            x.block(matlab::input_index(i),matlab::input_index(j),matlab::input_index(p),matlab::input_index(q)) = y;
          },
        py::keep_alive<0,1>(),
        DOC_TO_BE_DEFINED)

      .def("set_block", [](S& x, Index_type i, Index_type j, Index_type p, Index_type q, const Eigen::Matrix<T,1,-1>& y)
          {
            matlab::test_integer(i,j);
            matlab::test_integer(p,q);
            x.block(matlab::input_index(i),matlab::input_index(j),matlab::input_index(p),matlab::input_index(q)) = y;
          },
        py::keep_alive<0,1>(),
        DOC_TO_BE_DEFINED)

      .def("set_col", [](S& x, Index_type i, const Eigen::Matrix<T,-1,1>& y)
          {
            matlab::test_integer(i);
            x.col(matlab::input_index(i)) = y;
          },
        DOC_TO_BE_DEFINED)

      .def("set_col", [](S& x, Index_type i, const Eigen::Matrix<T,-1,-1>& y)
          {
            assert_release(y.cols() == 1);
            matlab::test_integer(i);
            x.col(matlab::input_index(i)) = y;
          },
        DOC_TO_BE_DEFINED)

      .def("set_row", [](S& x, Index_type i, const Eigen::Matrix<T,1,-1>& y)
          {
            matlab::test_integer(i);
            x.row(matlab::input_index(i)) = y;
          },
        DOC_TO_BE_DEFINED)

      .def("set_row", [](S& x, Index_type i, const Eigen::Matrix<T,-1,-1>& y)
          {
            assert_release(y.rows() == 1);
            matlab::test_integer(i);
            x.row(matlab::input_index(i)) = y;
          },
        DOC_TO_BE_DEFINED)

      .def("__call__", [](S& x, Index_type i, Index_type j) -> T&
          {
            matlab::test_integer(i,j);
            return x(matlab::input_index(i),matlab::input_index(j));
          }, py::return_value_policy::reference_internal,
        MATRIX_ADDONS_BASE_SCALAR_REF_OPERATORCALL_INDEX_INDEX)

      .def("resize", [](S& x, Index_type nb_rows, Index_type nb_cols)
          {
            matlab::test_integer(nb_rows, nb_cols);
            x.resize(nb_rows, nb_cols);
          },
        DOC_TO_BE_DEFINED,
        "nb_rows"_a, "nb_cols"_a)

      .def("resize_save_values", [](S& x, Index_type nb_rows, Index_type nb_cols)
          {
            matlab::test_integer(nb_rows, nb_cols);
            x.resize_save_values(nb_rows, nb_cols);
          },
        MATRIX_ADDONS_MATRIXBASE_VOID_RESIZE_SAVE_VALUES_INDEX_INDEX,
        "nb_rows"_a, "nb_cols"_a)

      .def_static("zero", [](Index_type r, Index_type c)
          {
            matlab::test_integer(r,c);
            return S::zero(r,c);
          },
        MATRIX_ADDONS_MATRIXBASE_STATIC_MATRIX_SCALARRC_ZERO_INDEX_INDEX,
        "r"_a, "c"_a)
      
      .def_static("ones", [](Index_type r, Index_type c)
          {
            matlab::test_integer(r,c);
            return S::ones(r,c);
          },
        MATRIX_ADDONS_MATRIXBASE_STATIC_MATRIX_SCALARRC_ONES_INDEX_INDEX,
        "r"_a, "c"_a)
      
      .def_static("constant", [](Index_type r, Index_type c, const T& x)
          {
            matlab::test_integer(r,c);
            return S::constant(r,c,x);
          },
        MATRIX_ADDONS_MATRIXBASE_STATIC_MATRIX_SCALARRC_CONSTANT_INDEX_INDEX_CONST_SCALAR_REF,
        "r"_a, "c"_a, "x"_a)
      
      .def_static("eye", [](Index_type r, Index_type c)
          {
            matlab::test_integer(r,c);
            return S::eye(r,c);
          },
        MATRIX_ADDONS_MATRIXBASE_STATIC_MATRIX_SCALARRC_EYE_INDEX_INDEX,
        "r"_a, "c"_a)
      
      .def_static("random", [](Index_type r, Index_type c)
          {
            matlab::test_integer(r,c);
            return S::random(r,c);
          },
        MATRIX_ADDONS_MATRIXBASE_STATIC_MATRIX_SCALARRC_RANDOM_INDEX_INDEX,
        "r"_a, "c"_a)

    ;
  }
  
  m.def("abs", [](const S& x) { return abs(x); },
    AUTO_ABS_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF);
  
  if constexpr(std::is_same_v<T,double>)
  {
    m.def("floor", [](const S& x) -> S { return floor(x); },
      AUTO_FLOOR_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF);
    
    m.def("ceil", [](const S& x) -> S { return ceil(x); },
      AUTO_CEIL_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF);
    
    m.def("round", [](const S& x) -> S { return round(x); },
      AUTO_ROUND_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF);

    pyclass.def("is_nan", [](const S& x)
        {
          return x.is_nan();
        },
      MATRIXBASE_ADDONS_BASE_BOOL_IS_NAN_CONST);
  }
}