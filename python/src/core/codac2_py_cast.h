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

  // Casting to trajectory types

    template<typename T>
      requires std::is_same_v<T,AnalyticTraj<VectorType>>
    bool is_instance(const py::object& x)
    {
      const py::object& x_traj = x.attr("traj");
      return x_traj && py::isinstance<T>(x_traj);
    }

    template<typename T>
      requires std::is_same_v<T,AnalyticTraj<VectorType>>
    const T& cast(const py::object& x)
    {
      assert(is_instance<T>(x));
      const py::object& x_traj = x.attr("traj");
      return x_traj.cast<const T&>();
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

    template<typename T>
      requires (std::is_same_v<AnalyticFunction<ScalarType>,T>
        || std::is_same_v<AnalyticFunction<VectorType>,T>
        || std::is_same_v<AnalyticFunction<MatrixType>,T>)
    bool is_instance(const py::object& x)
    {
      const py::object& f_ = x.attr("f");
      return f_ && py::isinstance<T>(f_);
    }

    template<typename T>
      requires (std::is_same_v<AnalyticFunction<ScalarType>,T>
        || std::is_same_v<AnalyticFunction<VectorType>,T>
        || std::is_same_v<AnalyticFunction<MatrixType>,T>)
    const T& cast(const py::object& x)
    {
      assert(is_instance<T>(x));
      const py::object& f_ = x.attr("f");
      return f_.cast<const T&>();
    }

    template<typename T>
      requires (std::is_same_v<SlicedTube<Interval>,T>
        || std::is_same_v<SlicedTube<IntervalVector>,T>
        || std::is_same_v<SlicedTube<IntervalMatrix>,T>)
    bool is_instance(const py::object& x)
    {
      const py::object& x_ = x.attr("tube");
      return x_ && py::isinstance<T>(x_);
    }

    template<typename T>
      requires (std::is_same_v<SlicedTube<Interval>,T>
        || std::is_same_v<SlicedTube<IntervalVector>,T>
        || std::is_same_v<SlicedTube<IntervalMatrix>,T>)
    const T& cast(const py::object& x)
    {
      assert(is_instance<T>(x));
      const py::object& x_ = x.attr("tube");
      return x_.cast<const T&>();
    }
}