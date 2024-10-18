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
#include <codac2_Vector.h>
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

    .def("__len__", [](const S& x)
        {
          return x.size();
        },
      SIZET_MATRIXBASE_ST_SIZE_CONST)

    .def("size", [](const S& x)
        {
          return x.size();
        },
      SIZET_MATRIXBASE_ST_SIZE_CONST)

    .def("nb_rows", [](const S& x)
        {
          return x.nb_rows();
        },
      SIZET_MATRIXBASE_ST_NB_ROWS_CONST)

    .def("nb_cols", [](const S& x)
        {
          return x.nb_cols();
        },
      SIZET_MATRIXBASE_ST_NB_COLS_CONST)

    .def("min_coeff", [](const S& x)
        {
          return x.min_coeff();
        },
      T_MATRIXBASE_ST_MIN_COEFF_CONST)

    .def("max_coeff", [](const S& x)
        {
          return x.max_coeff();
        },
      T_MATRIXBASE_ST_MAX_COEFF_CONST)

  ;

  if constexpr(!VECTOR_INHERITANCE)
  {
    pyclass

    .def("is_squared", [](const S& x)
        {
          return x.is_squared();
        },
      BOOL_MATRIXBASE_ST_IS_SQUARED_CONST)

    .def("__getitem__", [](const S& x, const py::tuple& ij) -> const T&
        {
          if constexpr(FOR_MATLAB)
            assert_release(py::isinstance<py::int_>(ij[0]) && py::isinstance<py::int_>(ij[1]));

          int i = ij[0].cast<int>();
          int j = ij[1].cast<int>();

          return x(matlab::input_index(i), matlab::input_index(j));
        }, py::return_value_policy::reference_internal,
      CONST_T_REF_MATRIXBASE_ST_OPERATORCALL_SIZET_SIZET_CONST)

    .def("__setitem__", [](S& x, const py::tuple& ij, const T& a)
        {
          if constexpr(FOR_MATLAB)
            assert_release(py::isinstance<py::int_>(ij[0]) && py::isinstance<py::int_>(ij[1]));

          int i = ij[0].cast<int>();
          int j = ij[1].cast<int>();

          x(matlab::input_index(i), matlab::input_index(j)) = a;
        },
      T_REF_MATRIXBASE_ST_OPERATORCALL_SIZET_SIZET)

    ;
  }

  pyclass

    .def("init", [](S& x, const T& a)
        {
          x.init(a);
        },
      VOID_MATRIXBASE_ST_INIT_CONST_T_REF,
      "x"_a)

    .def("init", [](S& x, const S& a)
        {
          x.init(a);
        },
      VOID_MATRIXBASE_ST_INIT_CONST_S_REF,
      "x"_a)

    .def("__repr__", [](const S& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_MATRIXBASE_ST_REF)
  ;

  if constexpr(!VECTOR_INHERITANCE)
  {
    // Virtual inheritance of VectorBase would shaddow the following methods.
    // Therefore they are not defined in case of this inheritance.

    pyclass

      .def("block", [](S& x, size_t_type i, size_t_type j, size_t_type p, size_t_type q) -> MatrixBaseBlock<EigenMatrix<T>&,T>
          {
            matlab::test_integer(i,j);
            matlab::test_integer(p,q);
            return x.block(matlab::input_index(i),matlab::input_index(j),matlab::input_index(p),matlab::input_index(q));
          },
        py::keep_alive<0,1>(),
        MATRIXBASEBLOCK_EIGENMATRIX_T_REFT_MATRIXBASE_ST_BLOCK_SIZET_SIZET_SIZET_SIZET)

      .def("col", [](S& x, size_t_type i) -> MatrixBaseBlock<EigenMatrix<T>&,T>
          {
            matlab::test_integer(i);
            return x.col(matlab::input_index(i));
          },
        py::keep_alive<0,1>(),
        MATRIXBASEBLOCK_EIGENMATRIX_T_REFT_MATRIXBASE_ST_COL_SIZET)

      .def("row", [](S& x, size_t_type i) -> MatrixBaseBlock<EigenMatrix<T>&,T>
          {
            matlab::test_integer(i);
            return x.row(matlab::input_index(i));
          },
        py::keep_alive<0,1>(),
        MATRIXBASEBLOCK_EIGENMATRIX_T_REFT_MATRIXBASE_ST_ROW_SIZET)

      .def("__call__", [](S& x, size_t_type i, size_t_type j) -> T&
          {
            matlab::test_integer(i,j);
            return x(matlab::input_index(i),matlab::input_index(j));
          }, py::return_value_policy::reference_internal,
        T_REF_MATRIXBASE_ST_OPERATORCALL_SIZET_SIZET)

      .def("resize", [](S& x, size_t_type nb_rows, size_t_type nb_cols)
          {
            matlab::test_integer(nb_rows, nb_cols);
            x.resize(nb_rows, nb_cols);
          },
        VOID_MATRIXBASE_ST_RESIZE_SIZET_SIZET,
        "nb_rows"_a, "nb_cols"_a)

      .def_static("zeros", [](size_t_type r, size_t_type c)
          {
            matlab::test_integer(r,c);
            return S::zeros(r,c);
          },
        STATIC_S_MATRIXBASE_ST_ZEROS_SIZET_SIZET,
        "r"_a, "c"_a)
      
      .def_static("ones", [](size_t_type r, size_t_type c)
          {
            matlab::test_integer(r,c);
            return S::ones(r,c);
          },
        STATIC_S_MATRIXBASE_ST_ONES_SIZET_SIZET,
        "r"_a, "c"_a)
      
      .def_static("eye", [](size_t_type r, size_t_type c)
          {
            matlab::test_integer(r,c);
            return S::ones(r,c);
          },
        STATIC_S_MATRIXBASE_ST_EYE_SIZET_SIZET,
        "r"_a, "c"_a)

    ;
  }
  
  //S abs(const MatrixBase<S,T>& x)
  m.def("abs", [](const S& x) { return abs(x); },
    S_ABS_CONST_MATRIXBASE_ST_REF);
}