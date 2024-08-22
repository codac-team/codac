/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include <codac2_Sep.h>
#include <codac2_IntervalVector.h>

using namespace codac2;
namespace py = pybind11;
using namespace py::literals;


class pySep : public Sep
{
  public:

    pySep(size_t_type n)
      : Sep(n), _n(n)
    { }

    // Trampoline (need one for each virtual function)
    BoxPair separate(const IntervalVector& x) const override
    {
      py::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      py::function overload = py::get_overload(this, "separate");
      assert_release(overload && "Sep: separate method not found");

      auto x_copy = IntervalVector(x);

      auto obj = overload(x_copy); // calls the Python function
      assert_release(py::isinstance<BoxPair>(obj) &&
        "Sep: error with separate method, incorrect returned Python type");

      return obj.cast<BoxPair>();
    }

    // Trampoline (need one for each virtual function)
    virtual std::shared_ptr<Sep> copy() const override
    {
      // Try to look up the overloaded method on the Python side
      py::function overload = py::get_overload(this, "copy");
      assert(overload && "Sep: copy method not found");

      auto obj = overload();
      return std::shared_ptr<Sep>(obj.cast<Sep*>(), [](auto p) { /* no delete */ });
    }

  protected:

    const size_t_type _n;
};