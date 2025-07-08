/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_py_matlab.h"
#include "codac2_arith_add.h"
#include "codac2_arith_sub.h"
#include "codac2_arith_mul.h"
#include "codac2_arith_div.h"
#include "codac2_py_AnalyticExprWrapper_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_pow.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

inline void export_ScalarExpr(py::module& m)
{
  py::class_<ScalarExpr>
    exported(m, "ScalarExpr");

  exported

    .def(py::init<int>())
    .def(py::init<double>())
    .def(py::init<Interval>())
    .def(py::init<ScalarExpr>())
    .def(py::init<ScalarVar>())

    .def("__pos__",  [](const ScalarExpr& e1) { return e1; }, py::is_operator())
    .def(py::self + py::self)
    .def("__add__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1+e2; }, py::is_operator())
    .def("__radd__", [](const ScalarExpr& e1, const Interval& e2)   { return e2+e1; }, py::is_operator())

    .def(- py::self)
    .def(py::self - py::self)
    .def("__sub__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1-e2; }, py::is_operator())
    .def("__rsub__", [](const ScalarExpr& e1, const Interval& e2)   { return e2-e1; }, py::is_operator())

    .def(py::self * py::self)
    .def("__mul__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1*e2; }, py::is_operator())
    .def("__rmul__", [](const ScalarExpr& e1, const Interval& e2)   { return e2*e1; }, py::is_operator())
    .def("__mul__",  [](const ScalarExpr& e1, const VectorExpr& e2) { return e1*e2; }, py::is_operator())

    .def("__truediv__",  [](const ScalarExpr& e1, const ScalarExpr& e2) { return e1/e2; }, py::is_operator())
    .def("__truediv__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1/e2; }, py::is_operator())
    .def("__rtruediv__", [](const ScalarExpr& e1, const Interval& e2)   { return e2/e1; }, py::is_operator())

    .def("__xor__",  [](const ScalarExpr& e1, const ScalarExpr& e2) { return e1^e2; }, py::is_operator())
    .def("__xor__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1^e2; }, py::is_operator())
    .def("__rxor__", [](const ScalarExpr& e1, const Interval& e2)   { return e2^e1; }, py::is_operator())

    .def("__pow__",  [](const ScalarExpr& e1, const ScalarExpr& e2) { return e1^e2; }, py::is_operator())
    .def("__pow__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1^e2; }, py::is_operator())
    .def("__rpow__", [](const ScalarExpr& e1, const Interval& e2)   { return e2^e1; }, py::is_operator())

  ;

  py::implicitly_convertible<int,ScalarExpr>();
  py::implicitly_convertible<double,ScalarExpr>();
  py::implicitly_convertible<Interval,ScalarExpr>();
  py::implicitly_convertible<ScalarVar,ScalarExpr>();
}

inline void export_VectorExpr(py::module& m)
{
  py::class_<VectorExpr>
    exported(m, "VectorExpr");

  exported

    .def(py::init<Vector>())
    .def(py::init<IntervalVector>())
    .def(py::init<VectorExpr>())
    .def(py::init<VectorVar>())

    .def(
        #if FOR_MATLAB
          "__call__"
        #else
          "__getitem__"
        #endif
        , [](const VectorExpr& x, Index_type i)
        {
          matlab::test_integer(i);
          return x[matlab::input_index(i)];
        }, py::return_value_policy::reference_internal,
      ANALYTICEXPRWRAPPER_SCALARTYPE_ANALYTICEXPRWRAPPER_T_OPERATORCOMPO_INDEX_CONST)

    .def("subvector", [](const VectorExpr& v, Index_type i, Index_type j)
        {
          matlab::test_integer(i, j);
          return v.subvector(matlab::input_index(i),matlab::input_index(j));
        },
      ANALYTICEXPRWRAPPER_VECTORTYPE_ANALYTICEXPRWRAPPER_T_SUBVECTOR_INDEX_INDEX_CONST)

    .def("__pos__",  [](const VectorExpr& e1)                           { return e1; }, py::is_operator())
    .def(py::self + py::self)
    .def("__add__",  [](const VectorExpr& e1, const IntervalVector& e2) { return e1+e2; }, py::is_operator())
    .def("__radd__", [](const VectorExpr& e1, const IntervalVector& e2) { return e1+e2; }, py::is_operator())

    .def(- py::self)
    .def(py::self - py::self)
    .def("__sub__",  [](const VectorExpr& e1, const IntervalVector& e2) { return e1-e2; }, py::is_operator())
    .def("__rsub__", [](const VectorExpr& e1, const IntervalVector& e2) { return e2-e1; }, py::is_operator())

    .def("__mul__",  [](const VectorExpr& e1, const ScalarExpr& e2)     { return e1*e2; }, py::is_operator())
    .def("__rmul__", [](const VectorExpr& e1, const ScalarExpr& e2)     { return e2*e1; }, py::is_operator())
    .def("__mul__",  [](const VectorExpr& e1, const Interval& e2)       { return e1*e2; }, py::is_operator())
    .def("__rmul__", [](const VectorExpr& e1, const Interval& e2)       { return e2*e1; }, py::is_operator())
    .def("__rmul__", [](const VectorExpr& e1, const MatrixExpr& e2)     { return e2*e1; }, py::is_operator())

    .def("__truediv__",  [](const VectorExpr& e1, const ScalarExpr& e2) { return e1/e2; }, py::is_operator())
    .def("__truediv__",  [](const VectorExpr& e1, const Interval& e2)   { return e1/e2; }, py::is_operator())

  ;

  py::implicitly_convertible<Vector,VectorExpr>();
  py::implicitly_convertible<IntervalVector,VectorExpr>();
  py::implicitly_convertible<VectorVar,VectorExpr>();
}

inline void export_MatrixExpr(py::module& m)
{
  py::class_<MatrixExpr>
    exported(m, "MatrixExpr");

  exported

    .def(py::init<Matrix>())
    .def(py::init<IntervalMatrix>())
    .def(py::init<MatrixExpr>())
    .def(py::init<MatrixVar>())

    .def("__call__", [](const MatrixExpr& x, Index_type i, Index_type j)
        {
          matlab::test_integer(i,j);
          return x(matlab::input_index(i),matlab::input_index(j));
        }, py::return_value_policy::reference_internal,
      ANALYTICEXPRWRAPPER_SCALARTYPE_ANALYTICEXPRWRAPPER_T_OPERATORCALL_INDEX_INDEX_CONST)

    .def("__pos__",  [](const MatrixExpr& e1)                           { return e1; }, py::is_operator())
    .def(py::self + py::self)
    .def("__add__",  [](const MatrixExpr& e1, const IntervalMatrix& e2) { return e1+e2; }, py::is_operator())
    .def("__radd__", [](const MatrixExpr& e1, const IntervalMatrix& e2) { return e1+e2; }, py::is_operator())

    .def(- py::self)
    .def(py::self - py::self)
    .def("__sub__",  [](const MatrixExpr& e1, const IntervalMatrix& e2) { return e1-e2; }, py::is_operator())
    .def("__rsub__", [](const MatrixExpr& e1, const IntervalMatrix& e2) { return e2-e1; }, py::is_operator())

    .def(py::self * py::self)
    .def("__mul__",  [](const MatrixExpr& e1, const IntervalMatrix& e2) { return e1*e2; }, py::is_operator())
    .def("__mul__",  [](const MatrixExpr& e1, const VectorVar& e2)      { return e1*e2; }, py::is_operator())
    .def("__mul__",  [](const MatrixExpr& e1, const MatrixVar& e2)      { return e1*e2; }, py::is_operator())
    .def("__rmul__", [](const MatrixExpr& e1, const IntervalMatrix& e2) { return e2*e1; }, py::is_operator())
    .def("__rmul__", [](const MatrixExpr& e1, const MatrixVar& e2)      { return e2*e1; }, py::is_operator())
    .def("__rmul__", [](const MatrixExpr& e1, const ScalarExpr& e2)     { return e2*e1; }, py::is_operator())
    .def("__rmul__", [](const MatrixExpr& e1, const Interval& e2)       { return e2*e1; }, py::is_operator())
    .def("__rmul__", [](const MatrixExpr& e1, const ScalarVar& e2)      { return e2*e1; }, py::is_operator())

    .def("__truediv__", [](const MatrixExpr& e1, const ScalarExpr& e2)  { return e1/e2; }, py::is_operator())
    .def("__truediv__", [](const MatrixExpr& e1, const Interval& e2)    { return e1/e2; }, py::is_operator())
    .def("__truediv__", [](const MatrixExpr& e1, const ScalarVar& e2)   { return e1/e2; }, py::is_operator())

  ;

  py::implicitly_convertible<Matrix,MatrixExpr>();
  py::implicitly_convertible<IntervalMatrix,MatrixExpr>();
  py::implicitly_convertible<MatrixVar,MatrixExpr>();
}