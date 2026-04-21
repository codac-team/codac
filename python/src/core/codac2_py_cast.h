/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <pybind11/pybind11.h>
#include <codac2_AnalyticTraj.h>
#include <codac2_SlicedTube.h>

namespace py = pybind11;
using namespace pybind11::literals;

namespace codac2
{
  // Common caster

    template<typename T>
    bool is_instance(const py::object& x)
    {
      return py::isinstance<T>(x);
    }

    template<typename T>
    const T& cast(const py::object& x)
    {
      if(!is_instance<T>(x))
      { assert_release("cast error"); }
      return x.cast<const T&>();
    }

  // Casting to function/expression types

    template<typename T>
      requires std::is_same_v<T,ScalarExpr>
    const T& cast(const py::object& x)
    {
      if(!is_instance<T>(x))
      { assert("unable to cast the provided py::object into a ScalarExpr"); }
      return x.cast<const T&>();
    }

    template<typename T>
      requires std::is_same_v<T,std::shared_ptr<ExprBase>>
    T cast(const py::object& x)
    {
      if(!py::isinstance<ScalarExpr>(x)
        && !py::isinstance<VectorExpr>(x)
        && !py::isinstance<MatrixExpr>(x))
      { assert("unable to cast the provided py::object into a ExprBase"); }

      if(py::isinstance<ScalarExpr>(x))
        return x.cast<ScalarExpr>()->copy();

      else if(py::isinstance<VectorExpr>(x))
        return x.cast<VectorExpr>()->copy();

      else
        return x.cast<MatrixExpr>()->copy();
    }
}