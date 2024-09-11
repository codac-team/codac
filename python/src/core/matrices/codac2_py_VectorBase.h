/** 
 *  VectorBase binding
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

#include "codac2_py_MatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename S,typename M,typename T>
void export_VectorBase(py::module& m, py::class_<S>& pyclass)
{
  //export_MatrixBase<S,T,true>(m, pyclass);
  // ^ We do not "inherit" from export_MatrixBase here, in order to
  // avoid double inheritance from IntervalVector or IntervalMatrix.
  // Inheritance is compensated in Vector binding.

  pyclass

    .def("__getitem__", [](const S& x, size_t_type index) -> const T&
        {
          matlab::test_integer(index);
          return x[matlab::input_index(index)];
        }, py::return_value_policy::reference_internal,
      CONST_T_REF_VECTORBASE_SMT_OPERATORCOMPO_SIZET_CONST)

    .def("__setitem__", [](S& x, size_t_type index, const T& a)
        {
          matlab::test_integer(index);
          x[matlab::input_index(index)] = a;
        },
      T_REF_VECTORBASE_SMT_OPERATORCOMPO_SIZET)

    .def("subvector", [](const S& x, size_t_type start_id, size_t_type end_id)
        {
          matlab::test_integer(start_id, end_id);
          return x.subvector(matlab::input_index(start_id), matlab::input_index(end_id));
        },
      S_VECTORBASE_SMT_SUBVECTOR_SIZET_SIZET_CONST,
      "start_id"_a, "end_id"_a)

    .def("resize", [](S& x, size_t_type n)
        {
          matlab::test_integer(n);
          x.resize(n);
        },
      VOID_VECTORBASE_SMT_RESIZE_SIZET,
      "n"_a)

    .def("put", [](S& x, size_t_type start_id, const S& x1)
        {
          matlab::test_integer(start_id);
          x.put(matlab::input_index(start_id), x1);
        },
      VOID_VECTORBASE_SMT_PUT_SIZET_CONST_S_REF,
      "start_id"_a, "x"_a)

    .def("transpose", &S::transpose,
      M_VECTORBASE_SMT_TRANSPOSE_CONST)

    .def("diag_matrix", &S::diag_matrix,
      M_VECTORBASE_SMT_DIAG_MATRIX_CONST)
    
    .def_static("zeros", [](size_t_type n)
        {
          matlab::test_integer(n);
          return S::zeros(n);
        },
      STATIC_S_VECTORBASE_SMT_ZEROS_SIZET,
      "n"_a)
    
    .def_static("ones", [](size_t_type n)
        {
          matlab::test_integer(n);
          return S::ones(n);
        },
      STATIC_S_VECTORBASE_SMT_ONES_SIZET,
      "n"_a)

    .def("__repr__", [](const S& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_VECTORBASE_SMT_REF)

    .def("__iter__", [](const S &x)
        {
          auto x_ = x._e.reshaped();
          return py::make_iterator(x_.begin(), x_.end());
        },
      py::keep_alive<0, 1>() /*  keep object alive while iterator exists */)
  ;
}