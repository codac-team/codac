/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
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
struct ExprWrapper : public std::shared_ptr<codac2::AnalyticExpr<T>>
{
  ExprWrapper(const codac2::ScalarVar& e)
    : std::shared_ptr<codac2::AnalyticExpr<T>>(std::dynamic_pointer_cast<codac2::AnalyticExpr<T>>(e.copy()))
  {
    static_assert(std::is_same_v<T,codac2::ScalarOpValue>);
  }

  ExprWrapper(const codac2::VectorVar& e)
    : std::shared_ptr<codac2::AnalyticExpr<T>>(std::dynamic_pointer_cast<codac2::AnalyticExpr<T>>(e.copy()))
  {
    static_assert(std::is_same_v<T,codac2::VectorOpValue>);
  }

  ExprWrapper(const std::shared_ptr<codac2::AnalyticExpr<T>>& e)
    : std::shared_ptr<codac2::AnalyticExpr<T>>(e)
  { }
};

template<typename T>
void export_ExprWrapper(py::module& m, const std::string& export_name)
{
  py::class_<ExprWrapper<T>>
    exported(m, export_name.c_str());

  exported.def(py::init<std::shared_ptr<AnalyticExpr<T>>>());
  
  if constexpr(std::is_same_v<T,ScalarOpValue>)
    exported.def(py::init<ScalarVar>(),
      "Converting a ScalarVar into an ExprWrapper (binding object).");
  
  if constexpr(std::is_same_v<T,VectorOpValue>)
    exported.def(py::init<VectorVar>(),
      "Converting a VectorVar into an ExprWrapper (binding object).");

  if constexpr(std::is_same_v<T,ScalarOpValue>)
  {
    exported
    
      .def("__add__", [](const ExprWrapper<T>& e1) { return ExprWrapper<T>(+e1); })
      .def("__add__", [](const ExprWrapper<T>& e1, const Interval& e2) { return ExprWrapper<T>(e1 + e2); })
      .def("__add__", [](const Interval& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 + e2); })
      .def("__add__", [](const ExprWrapper<T>& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 + e2); })

      .def("__sub__", [](const ExprWrapper<T>& e1) { return ExprWrapper<T>(-e1); })
      .def("__sub__", [](const ExprWrapper<T>& e1, const Interval& e2) { return ExprWrapper<T>(e1 - e2); })
      .def("__sub__", [](const Interval& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 - e2); })
      .def("__sub__", [](const ExprWrapper<T>& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 - e2); })

      .def("__mul__", [](const ExprWrapper<T>& e1, const Interval& e2) { return ExprWrapper<T>(e1 * e2); })
      .def("__mul__", [](const Interval& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 * e2); })
      .def("__mul__", [](const ExprWrapper<T>& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 * e2); })

      .def("__div__", [](const ExprWrapper<T>& e1, const Interval& e2) { return ExprWrapper<T>(e1 / e2); })
      .def("__div__", [](const Interval& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 / e2); })
      .def("__div__", [](const ExprWrapper<T>& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 / e2); })

    ;
  }

  if constexpr(std::is_same_v<T,VectorOpValue>)
  {
    exported
    
      .def("__add__", [](const ExprWrapper<T>& e1) { return ExprWrapper<T>(+e1); })
      .def("__add__", [](const ExprWrapper<T>& e1, const IntervalVector& e2) { return ExprWrapper<T>(e1 + e2); })
      .def("__add__", [](const IntervalVector& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 + e2); })
      .def("__add__", [](const ExprWrapper<T>& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 + e2); })

      .def("__sub__", [](const ExprWrapper<T>& e1) { return ExprWrapper<T>(-e1); })
      .def("__sub__", [](const ExprWrapper<T>& e1, const IntervalVector& e2) { return ExprWrapper<T>(e1 - e2); })
      .def("__sub__", [](const IntervalVector& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 - e2); })
      .def("__sub__", [](const ExprWrapper<T>& e1, const ExprWrapper<T>& e2) { return ExprWrapper<T>(e1 - e2); })

      .def("__mul__", [](const ExprWrapper<ScalarOpValue>& e1, const ExprWrapper<VectorOpValue>& e2) { return ExprWrapper<VectorOpValue>(e1 * e2); })
      .def("__mul__", [](const Interval& e1, const ExprWrapper<VectorOpValue>& e2) { return ExprWrapper<VectorOpValue>(e1 * e2); })
      .def("__mul__", [](const ExprWrapper<ScalarOpValue>& e1, const IntervalVector& e2) { return ExprWrapper<VectorOpValue>(e1 * e2); })

    ;
  }
}