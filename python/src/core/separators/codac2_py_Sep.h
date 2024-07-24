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
using namespace pybind11::literals;


class pySep : public Sep
{
  public:

    pySep(size_t_type n)
      : Sep(n), _n(n)
    { }

    // Trampoline (need one for each virtual function)
    BoxPair separate(const IntervalVector& x) const override
    {
      pybind11::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      pybind11::function overload = pybind11::get_overload(this, "separate");

      if(overload) // method is found
      {
        auto x_copy = IntervalVector(x);
        auto obj = overload(x_copy); // calls the Python function

        if(pybind11::isinstance<BoxPair>(obj)) // checks if it returned a correct Python type
          return obj.cast<BoxPair>();

        else
        {
          assert(false && "Sep: error with separate method");
        }
      }

      else
      {
        assert(false && "Sep: separate method not found");
      }

      return { x,x }; // should not reach this point
    }

    // Trampoline (need one for each virtual function)
    virtual std::shared_ptr<Sep> copy() const override
    {
      // Try to look up the overloaded method on the Python side
      pybind11::function overload = pybind11::get_overload(this, "copy");

      if(overload) // method is found
      {
        auto obj = overload();
        return std::shared_ptr<Sep>(obj.cast<Sep*>(), [](auto p) { /* no delete */ });
      }

      else
      {
        assert(false && "Sep: copy method not found");
        return nullptr;
      }
    }

  protected:

    const size_t_type _n;
};