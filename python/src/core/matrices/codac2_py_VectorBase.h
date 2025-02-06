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
void export_VectorBase([[maybe_unused]] py::module& m, py::class_<S>& pyclass)
{
  //export_MatrixBase<S,T,true>(m, pyclass);
  // ^ We do not "inherit" from export_MatrixBase here, in order to
  // avoid double inheritance from IntervalVector or IntervalMatrix.
  // Inheritance is compensated in Vector binding.

  pyclass

    .def(py::init(
        [](const Eigen::Matrix<T,-1,-1>& x)
        {
          return std::make_unique<S>(x);
        }),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def("__getitem__", [](const S& x, Index_type index) -> const T&
        {
          matlab::test_integer(index);
          return x[matlab::input_index(index)];
        }, py::return_value_policy::reference_internal,
      MATRIX_ADDONS_VECTORBASE_CONST_SCALAR_REF_OPERATORCOMPO_INDEX_CONST)

    .def("__setitem__", [](S& x, Index_type index, const T& a)
        {
          matlab::test_integer(index);
          x[matlab::input_index(index)] = a;
        },
      MATRIX_ADDONS_VECTORBASE_SCALAR_REF_OPERATORCOMPO_INDEX)

    .def("subvector", [](const S& x, Index_type start_id, Index_type end_id) -> S
        {
          matlab::test_integer(start_id, end_id);
          return x.subvector(matlab::input_index(start_id), matlab::input_index(end_id));
        },
      MATRIXBASE_ADDONS_VECTORBASE_AUTO_SUBVECTOR_INDEX_INDEX_CONST,
      "start_id"_a, "end_id"_a)

    .def("resize", [](S& x, Index_type n)
        {
          matlab::test_integer(n);
          x.resize(n);
        },
      DOC_TO_BE_DEFINED,
      "n"_a)

    .def("resize_save_values", [](S& x, Index_type n)
        {
          matlab::test_integer(n);
          x.resize_save_values(n);
        },
      MATRIX_ADDONS_VECTORBASE_VOID_RESIZE_SAVE_VALUES_INDEX,
      "n"_a)

    .def("put", [](S& x, Index_type start_id, const S& x1)
        {
          matlab::test_integer(start_id);
          x.put(matlab::input_index(start_id), x1);
        },
      MATRIX_ADDONS_VECTORBASE_VOID_PUT_INDEX_CONST_MATRIXBASE_OTHERDERIVED_REF,
      "start_id"_a, "x"_a)
    
    .def_static("zero", [](Index_type n)
        {
          matlab::test_integer(n);
          return S::zero(n);
        },
      MATRIX_ADDONS_VECTORBASE_STATIC_MATRIX_SCALARRC_ZERO_INDEX,
      "n"_a)
    
    .def_static("ones", [](Index_type n)
        {
          matlab::test_integer(n);
          return S::ones(n);
        },
      MATRIX_ADDONS_VECTORBASE_STATIC_MATRIX_SCALARRC_ONES_INDEX,
      "n"_a)
    
    .def_static("constant", [](Index_type n, const T& x)
        {
          matlab::test_integer(n);
          return S::constant(n,x);
        },
      MATRIX_ADDONS_VECTORBASE_STATIC_MATRIX_SCALARRC_CONSTANT_INDEX_CONST_SCALAR_REF,
      "n"_a, "x"_a)
    
    .def_static("random", [](Index_type n)
        {
          matlab::test_integer(n);
          return S::random(n);
        },
      MATRIX_ADDONS_VECTORBASE_STATIC_MATRIX_SCALARRC_RANDOM_INDEX,
      "n"_a)

    .def("__repr__", [](const S& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      DOC_TO_BE_DEFINED)

    .def("__iter__", [](const S &x)
        {
          return py::make_iterator(x.begin(), x.end());
        },
      py::keep_alive<0, 1>() /*  keep object alive while iterator exists */)
  ;
}