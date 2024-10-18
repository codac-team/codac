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
#include <codac2_Sep.h>
#include <codac2_IntervalVector.h>
#include "codac2_py_matlab.h"

using namespace codac2;
namespace py = pybind11;
using namespace py::literals;


class pySep : public SepBase
{
  public:

    pySep(size_t_type n)
      : SepBase(n)
    {
      matlab::test_integer(n);
    }

    // Trampoline (need one for each virtual function)
    BoxPair separate(const IntervalVector& x) const override
    {
      py::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      py::function overload = py::get_overload(this, "separate");
      assert_release(overload && "SepBase: separate method not found");

      auto obj = overload(x); // calls the Python function
      assert_release((py::isinstance<BoxPair>(obj) || py::isinstance<py::list>(obj)) &&
        "SepBase: error with separate method, incorrect returned Python type");

      if(py::isinstance<BoxPair>(obj))
        return obj.cast<BoxPair>();

      else
      {
        assert_release(obj.cast<py::list>().size() == 2 && "SepBase: error with separate method, should return two boxes");
        return BoxPair(obj.cast<py::list>()[0].cast<IntervalVector>(), obj.cast<py::list>()[1].cast<IntervalVector>());
      }
    }

    // Trampoline (need one for each virtual function)
    virtual std::shared_ptr<SepBase> copy() const override
    {
      // Try to look up the overloaded method on the Python side
      py::function overload = py::get_overload(this, "copy");
      assert(overload && "SepBase: copy method not found");

      auto obj = overload();
      return std::shared_ptr<SepBase>(obj.cast<SepBase*>(), [](auto p) { /* no delete */ });
    }
};