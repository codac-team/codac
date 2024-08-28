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
#include "codac2_py_core.h"
#include <codac2_Vector.h>

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

    .def("__getitem__", [](const S& x, size_t_type index) -> const T&
        {
          matlab::test_integer(index);
          return x[matlab::input_index(index)];
        }, py::return_value_policy::reference_internal,
      CONST_T_REF_MATRIXBASE_ST_OPERATORCOMPO_SIZET_CONST)

    .def("__setitem__", [](S& x, size_t_type index, const T& a)
        {
          matlab::test_integer(index);
          x[matlab::input_index(index)] = a;
        },
      T_REF_MATRIXBASE_ST_OPERATORCOMPO_SIZET)

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
        MATRIXBASEBLOCK_EIGENMATRIX_T_REFT_MATRIXBASE_ST_BLOCK_SIZET_SIZET_SIZET_SIZET)

      .def("col", [](S& x, size_t_type i) -> MatrixBaseBlock<EigenMatrix<T>&,T>
          {
            matlab::test_integer(i);
            return x.col(matlab::input_index(i));
          },
        MATRIXBASEBLOCK_EIGENMATRIX_T_REFT_MATRIXBASE_ST_COL_SIZET)

      .def("row", [](S& x, size_t_type i) -> MatrixBaseBlock<EigenMatrix<T>&,T>
          {
            matlab::test_integer(i);
            return x.row(matlab::input_index(i));
          },
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

  // MatrixBase operations

  //const auto& operator+() const
  pyclass.def(+py::self,
    CONST_AUTO_REF_MATRIXBASE_ST_OPERATORPLUS_CONST);

  //S operator-() const
  pyclass.def(-py::self,
    S_MATRIXBASE_ST_OPERATORMINUS_CONST);

  //friend S operator+(const S& x1, const S& x2)
  pyclass.def(py::self + py::self,
    S_OPERATORPLUS_CONST_S_REF_CONST_S_REF);

  //friend S operator+(const S& x1, const MatrixBaseBlock<Q_,T_>& x2)
  pyclass.def("__add__", [](const S& x1, const MatrixBaseBlock<EigenMatrix<T>&,T>& x2) { return x1+x2; }, py::is_operator(),
    S_OPERATORPLUS_CONST_S_REF_CONST_MATRIXBASEBLOCK_Q_T__REF);

  //friend S operator+(const MatrixBaseBlock<Q_,T_>& x1, const S& x2)
  pyclass.def("__radd__", [](const S& x2, const MatrixBaseBlock<EigenMatrix<T>&,T>& x1) { return x1+x2; }, py::is_operator(),
    S_OPERATORPLUS_CONST_MATRIXBASEBLOCK_Q_T__REF_CONST_S_REF);

  //S& operator+=(const S& x)
  pyclass.def(py::self += py::self,
    S_REF_MATRIXBASE_ST_OPERATORPLUSEQ_CONST_S_REF);

  //S& operator+=(const MatrixBaseBlock<Q_,T_>& x)
  pyclass.def("__iadd__", [](S& x1, const MatrixBaseBlock<EigenMatrix<T>&,T>& x2) { return x1+=x2; }, py::is_operator(),
    S_REF_MATRIXBASE_ST_OPERATORPLUSEQ_CONST_MATRIXBASEBLOCK_Q_T__REF);

  //friend S operator-(const S& x1, const S& x2)
  pyclass.def(py::self - py::self,
    S_OPERATORMINUS_CONST_S_REF_CONST_S_REF);

  //friend S operator-(const S& x1, const MatrixBaseBlock<Q_,T_>& x2)
  pyclass.def("__sub__", [](const S& x1, const MatrixBaseBlock<EigenMatrix<T>&,T>& x2) { return x1-x2; }, py::is_operator(),
    S_OPERATORMINUS_CONST_S_REF_CONST_MATRIXBASEBLOCK_Q_T__REF);

  //friend S operator-(const MatrixBaseBlock<Q_,T_>& x1, const S& x2)
  pyclass.def("__rsub__", [](const S& x2, const MatrixBaseBlock<EigenMatrix<T>&,T>& x1) { return x1-x2; }, py::is_operator(),
    S_OPERATORMINUS_CONST_MATRIXBASEBLOCK_Q_T__REF_CONST_S_REF);

  //S& operator-=(const S& x)
  pyclass.def(py::self -= py::self,
    S_REF_MATRIXBASE_ST_OPERATORMINUSEQ_CONST_S_REF);

  //S& operator-=(const MatrixBaseBlock<Q_,T_>& x)
  pyclass.def("__isub__", [](S& x1, const MatrixBaseBlock<EigenMatrix<T>&,T>& x2) { return x1-=x2; }, py::is_operator(),
    S_REF_MATRIXBASE_ST_OPERATORMINUSEQ_CONST_MATRIXBASEBLOCK_Q_T__REF);

  //friend S operator*(const S& x1, const S& x2)
  pyclass.def(py::self * py::self,
    S_OPERATORMUL_CONST_S_REF_CONST_S_REF);

  //friend S operator*(const T& x1, const S& x2)
  pyclass.def("__rmul__", [](const S& x2, const T& x1) { return x1*x2; }, py::is_operator(),
    S_OPERATORMUL_CONST_T_REF_CONST_S_REF);

  if constexpr(!VECTOR_INHERITANCE)
  {
    //friend S_ operator*(const S& x1, const VectorBase<S_,S,T>& x2)
    pyclass.def("__mul__", [](const S& x1, const Vector& x2) { return x1*x2; }, py::is_operator(),
      S__OPERATORMUL_CONST_S_REF_CONST_VECTORBASE_S_ST_REF);
  }

  //friend S operator*(const S& x1, const MatrixBaseBlock<Q_,T_>& x2)
  pyclass.def("__mul__", [](const S& x1, const MatrixBaseBlock<EigenMatrix<T>&,T>& x2) { return x1*x2; }, py::is_operator(),
    S_OPERATORMUL_CONST_S_REF_CONST_MATRIXBASEBLOCK_Q_T__REF);

  //friend S operator*(const MatrixBaseBlock<Q_,T_>& x1, const S& x2)
  pyclass.def("__rmul__", [](const S& x2, const MatrixBaseBlock<EigenMatrix<T>&,T>& x1) { return x1*x2; }, py::is_operator(),
    S_OPERATORMUL_CONST_MATRIXBASEBLOCK_Q_T__REF_CONST_S_REF);

  //S& operator*=(const S& x)
  pyclass.def(py::self *= py::self,
    S_REF_MATRIXBASE_ST_OPERATORMULEQ_CONST_S_REF);

  //S& operator*=(const MatrixBaseBlock<Q_,T_>& x)
  pyclass.def("__imul__", [](S& x1, const MatrixBaseBlock<EigenMatrix<T>&,T>& x2) { return x1*=x2; }, py::is_operator(),
    S_REF_MATRIXBASE_ST_OPERATORMULEQ_CONST_MATRIXBASEBLOCK_Q_T__REF);

  //friend S operator/(const S& x1, const T& x2)
  pyclass.def("__truediv__", [](const S& x1, const T& x2) { return x1/x2; }, py::is_operator(),
    S_OPERATORDIV_CONST_S_REF_CONST_T_REF);

  //S abs(const MatrixBase<S,T>& x)
  m.def("abs", [](const S& x) { return abs(x); },
    S_ABS_CONST_MATRIXBASE_ST_REF);
}