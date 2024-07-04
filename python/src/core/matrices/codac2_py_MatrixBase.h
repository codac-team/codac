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

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename S,typename T,bool VECTOR_INHERITANCE>
void export_MatrixBase(py::class_<S>& pyclass)
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


/*

      MatrixBaseBlock<EigenMatrix<T>&,T> block(size_t i, size_t j, size_t p, size_t q)
      MatrixBaseBlock<EigenMatrix<T>&,T> col(size_t i)
      MatrixBaseBlock<EigenMatrix<T>&,T> row(size_t i)

      const auto& operator+() const
      S operator-() const
      friend S operator+(const S& x1, const S& x2)

      template<typename Q_,typename T_>
      friend S operator+(const S& x1, const MatrixBaseBlock<Q_,T_>& x2)

      template<typename Q_,typename T_>
      friend S operator+(const MatrixBaseBlock<Q_,T_>& x1, const S& x2)

      S& operator+=(const S& x)

      template<typename Q_,typename T_>
      S& operator+=(const MatrixBaseBlock<Q_,T_>& x)

      friend S operator-(const S& x1, const S& x2)

      template<typename Q_,typename T_>
      friend S operator-(const S& x1, const MatrixBaseBlock<Q_,T_>& x2)

      template<typename Q_,typename T_>
      friend S operator-(const MatrixBaseBlock<Q_,T_>& x1, const S& x2)

      S& operator-=(const S& x)

      template<typename Q_,typename T_>
      S& operator-=(const MatrixBaseBlock<Q_,T_>& x)

      friend S operator*(const S& x1, const S& x2)
      friend S operator*(const T& x1, const S& x2)

      template<typename S_>
      friend S_ operator*(const S& x1, const VectorBase<S_,S,T>& x2)

      template<typename Q_,typename T_>
      friend S operator*(const S& x1, const MatrixBaseBlock<Q_,T_>& x2)

      template<typename Q_,typename T_>
      friend S operator*(const MatrixBaseBlock<Q_,T_>& x1, const S& x2)

      S& operator*=(const S& x)

      template<typename Q_,typename T_>
      S& operator*=(const MatrixBaseBlock<Q_,T_>& x)

      friend S operator/(const S& x1, const T& x2)

      template<typename S_,typename T_>
      friend std::ostream& operator<<(std::ostream& os, const MatrixBase<S_,T_>& x);

  template<typename S,typename T>
  S abs(const MatrixBase<S,T>& x)

  template<typename S,typename T>
  std::ostream& operator<<(std::ostream& os, const MatrixBase<S,T>& x)

  template<typename Q,typename T>
  struct MatrixBaseBlock
  {
    Q _m;
    size_t _i,_j,_p,_q;

    MatrixBaseBlock(Q m, size_t i, size_t j, size_t p, size_t q);

    template<typename S_>
    void operator=(const MatrixBase<S_,T>& x);

    template<typename OtherDerived>
    void operator=(const Eigen::MatrixBase<OtherDerived>& x);

    auto eval() const;

    template<typename M>
    bool operator==(const M& x) const;
  };
  */

}