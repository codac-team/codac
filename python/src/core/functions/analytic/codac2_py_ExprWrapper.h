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
#include "codac2_analytic_operations.h"
#include "codac2_analytic_variables.h"
#include "codac2_AnalyticExpr.h"
#include "codac2_AnalyticFunction.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
struct ExprWrapper;

struct ExprWrapperBase
{
  ExprWrapperBase()
  { }

  ExprWrapperBase(const std::shared_ptr<ExprBase>& eb_)
    : eb(eb_)
  { }
  
  std::shared_ptr<ExprBase> eb = nullptr;
};

template<>
struct ExprWrapper<ScalarOpValue> : public ExprWrapperBase
{
  ExprWrapper(const Interval& e)
    : e(std::dynamic_pointer_cast<AnalyticExpr<ScalarOpValue>>(const_value(e)->copy()))
  { }

  ExprWrapper(const ScalarVar& e)
    : e(std::dynamic_pointer_cast<AnalyticExpr<ScalarOpValue>>(e.copy()))
  { }

  ExprWrapper(const ExprWrapper<ScalarOpValue>& e)
    : e(std::dynamic_pointer_cast<AnalyticExpr<ScalarOpValue>>(e.copy()))
  { }

  explicit ExprWrapper(const std::shared_ptr<AnalyticExpr<ScalarOpValue>>& e)
    : e(std::dynamic_pointer_cast<AnalyticExpr<ScalarOpValue>>(e->copy()))
  { }

  std::shared_ptr<AnalyticExpr<ScalarOpValue>> copy() const
  {
    return std::dynamic_pointer_cast<AnalyticExpr<ScalarOpValue>>(e->copy());
  }

  operator std::shared_ptr<AnalyticExpr<ScalarOpValue>>()
  {
    return copy();
  }

  virtual ExprWrapperBase raw_copy() const
  {
    return ExprWrapperBase(e->copy());
  }

  std::shared_ptr<AnalyticExpr<ScalarOpValue>> e;
};

using ScalarExpr = ExprWrapper<ScalarOpValue>;

// Scalar operations

inline const ScalarExpr& operator+(const ScalarExpr& e1)
{
  return e1;
}

inline ScalarExpr operator+(const ScalarExpr& e1, const ScalarExpr& e2)
{
  return ScalarExpr(e1.e+e2.e);
}

inline ScalarExpr operator-(const ScalarExpr& e1)
{
  return ScalarExpr(-e1.e);
}

inline ScalarExpr operator-(const ScalarExpr& e1, const ScalarExpr& e2)
{
  return ScalarExpr(e1.e-e2.e);
}

inline ScalarExpr operator*(const ScalarExpr& e1, const ScalarExpr& e2)
{
  return ScalarExpr(e1.e*e2.e);
}

inline ScalarExpr operator/(const ScalarExpr& e1, const ScalarExpr& e2)
{
  return ScalarExpr(e1.e/e2.e);
}

template<>
struct ExprWrapper<VectorOpValue> : public ExprWrapperBase
{
  ExprWrapper(const IntervalVector& e)
    : e(std::dynamic_pointer_cast<AnalyticExpr<VectorOpValue>>(const_value(e)->copy()))
  { }

  ExprWrapper(const VectorVar& e)
    : e(std::dynamic_pointer_cast<AnalyticExpr<VectorOpValue>>(e.copy()))
  { }

  ExprWrapper(const ExprWrapper<VectorOpValue>& e)
    : e(std::dynamic_pointer_cast<AnalyticExpr<VectorOpValue>>(e.copy()))
  { }

  explicit ExprWrapper(const std::shared_ptr<AnalyticExpr<VectorOpValue>>& e)
    : e(std::dynamic_pointer_cast<AnalyticExpr<VectorOpValue>>(e->copy()))
  { }

  std::shared_ptr<AnalyticExpr<VectorOpValue>> copy() const
  {
    return std::dynamic_pointer_cast<AnalyticExpr<VectorOpValue>>(e->copy());
  }

  operator std::shared_ptr<AnalyticExpr<VectorOpValue>>()
  {
    return copy();
  }

  virtual ExprWrapperBase raw_copy() const
  {
    return ExprWrapperBase(e->copy());
  }

  std::shared_ptr<AnalyticExpr<VectorOpValue>> e;
};

using VectorExpr = ExprWrapper<VectorOpValue>;

// Vector operations

inline const VectorExpr& operator+(const VectorExpr& e1)
{
  return e1;
}

inline VectorExpr operator+(const VectorExpr& e1, const VectorExpr& e2)
{
  return VectorExpr(e1.e+e2.e);
}

inline VectorExpr operator-(const VectorExpr& e1)
{
  return VectorExpr(-e1.e);
}

inline VectorExpr operator-(const VectorExpr& e1, const VectorExpr& e2)
{
  return VectorExpr(e1.e-e2.e);
}

inline VectorExpr operator*(const ScalarExpr& e1, const VectorExpr& e2)
{
  return VectorExpr(e1.e*e2.e);
}

inline VectorExpr operator*(const VectorExpr& e1, const ScalarExpr& e2)
{
  return VectorExpr(e1.e*e2.e);
}

inline VectorExpr operator/(const VectorExpr& e1, const ScalarExpr& e2)
{
  return VectorExpr(e1.e/e2.e);
}

inline void export_ExprWrapperBase(py::module& m)
{
  py::class_<ExprWrapperBase> py_wrap(m, "ExprWrapperBase");
  py_wrap  .def(py::init<const std::shared_ptr<ExprBase>&>());
}

inline void export_ScalarExpr(py::module& m)
{
  py::class_<ScalarExpr>
    exported(m, "ScalarExpr");

  exported

    .def(py::init<std::shared_ptr<AnalyticExpr<ScalarOpValue>>>())
    .def(py::init<ScalarExpr>())
    .def(py::init<Interval>())
    .def(py::init<ScalarVar>())
    .def("copy", &ScalarExpr::copy)
    .def("raw_copy", &ScalarExpr::raw_copy)

    .def("__pos__",  [](const ScalarExpr& e1) { return e1; }, py::is_operator())
    .def(py::self + py::self)
    .def("__radd__", [](const ScalarExpr& e1, const ScalarVar& e2)  { return e2+e1; }, py::is_operator())
    .def("__add__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1+e2; }, py::is_operator())
    .def("__radd__", [](const ScalarExpr& e1, const Interval& e2)   { return e2+e1; }, py::is_operator())

    .def(- py::self)
    .def(py::self - py::self)
    .def("__sub__",  [](const ScalarExpr& e1, const ScalarVar& e2)  { return e1-e2; }, py::is_operator())
    .def("__sub__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1-e2; }, py::is_operator())
    .def("__rsub__", [](const ScalarExpr& e1, const Interval& e2)   { return e2-e1; }, py::is_operator())

    .def(py::self * py::self)
    .def("__mul__",  [](const ScalarExpr& e1, const ScalarVar& e2)  { return e1*e2; }, py::is_operator())
    .def("__mul__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1*e2; }, py::is_operator())
    .def("__rmul__", [](const ScalarExpr& e1, const Interval& e2)   { return e2*e1; }, py::is_operator())
    .def("__mul__",  [](const ScalarExpr& e1, const VectorExpr& e2) { return e1*e2; }, py::is_operator())

    .def("__truediv__",  [](const ScalarExpr& e1, const ScalarExpr& e2) { return e1/e2; }, py::is_operator())
    .def("__truediv__",  [](const ScalarExpr& e1, const ScalarVar& e2)  { return e1/e2; }, py::is_operator())
    .def("__truediv__",  [](const ScalarExpr& e1, const Interval& e2)   { return e1/e2; }, py::is_operator())
    .def("__rtruediv__", [](const ScalarExpr& e1, const Interval& e2)   { return e2/e1; }, py::is_operator())

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

    .def(py::init<std::shared_ptr<AnalyticExpr<VectorOpValue>>>())
    .def(py::init<VectorExpr>())
    .def(py::init<IntervalVector>())
    .def(py::init<VectorVar>())
    .def("copy", &VectorExpr::copy)
    .def("raw_copy", &VectorExpr::raw_copy)

    .def("__pos__",  [](const VectorExpr& e1)                           { return e1; }, py::is_operator())
    .def(py::self + py::self)
    .def("__add__",  [](const VectorExpr& e1, const VectorVar& e2)      { return e1+e2; }, py::is_operator())
    .def("__add__",  [](const VectorExpr& e1, const IntervalVector& e2) { return e1+e2; }, py::is_operator())
    .def("__radd__", [](const VectorExpr& e1, const IntervalVector& e2) { return e1+e2; }, py::is_operator())

    .def(- py::self)
    .def(py::self - py::self)
    .def("__sub__",  [](const VectorExpr& e1, const VectorVar& e2)      { return e1-e2; }, py::is_operator())
    .def("__sub__",  [](const VectorExpr& e1, const IntervalVector& e2) { return e1-e2; }, py::is_operator())
    .def("__rsub__", [](const VectorExpr& e1, const IntervalVector& e2) { return e2-e1; }, py::is_operator())

    .def("__mul__",  [](const VectorExpr& e1, const ScalarExpr& e2)     { return e1*e2; }, py::is_operator())
    .def("__rmul__", [](const VectorExpr& e1, const ScalarExpr& e2)     { return e2*e1; }, py::is_operator())
    .def("__mul__",  [](const VectorExpr& e1, const Interval& e2)       { return e1*e2; }, py::is_operator())
    .def("__rmul__", [](const VectorExpr& e1, const Interval& e2)       { return e2*e1; }, py::is_operator())

    .def("__truediv__",  [](const VectorExpr& e1, const ScalarExpr& e2) { return e1/e2; }, py::is_operator())
    .def("__truediv__",  [](const VectorExpr& e1, const Interval& e2)   { return e1/e2; }, py::is_operator())

  ;

  py::implicitly_convertible<Vector,VectorExpr>();
  py::implicitly_convertible<IntervalVector,VectorExpr>();
  py::implicitly_convertible<VectorVar,VectorExpr>();
}