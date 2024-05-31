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
#include <codac2_Ctc.h>
#include <codac2_IntervalVector.h>

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


class pyCtcIntervalVector : public Ctc_<IntervalVector>
{
  public:

    pyCtcIntervalVector(size_t_type n)
      : Ctc_<IntervalVector>(n)
    { }

    // Trampoline (need one for each virtual function)
    void contract(IntervalVector& x) const override
    {
      pybind11::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      pybind11::function overload = pybind11::get_overload(this, "contract");

      if(overload) // method is found
      {
        auto x_copy = IntervalVector(x);
        auto obj = overload(x_copy); // calls the Python function

        if(pybind11::isinstance<IntervalVector>(obj))
        {
          // Checks if it returned a correct Python type
          x &= obj.cast<IntervalVector>(); // casts it and assigns it to the value
          return; // Return true; value should be used
        }

        else
        {
          assert(false && "Ctc: error with contract method");
        }
      }

      else
      {
        assert(false && "Ctc: contract method not found");
      }
    }

    // Trampoline (need one for each virtual function)
    virtual std::shared_ptr<Ctc> copy() const override
    {
      pybind11::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      pybind11::function overload = pybind11::get_overload(this, "copy");

      if(overload) // method is found
      {
        auto obj = overload();

        if(pybind11::isinstance<std::shared_ptr<Ctc>>(obj))
          return obj.cast<std::shared_ptr<Ctc>>();

        else
        {
          assert(false && "Ctc: error with copy method");
          return nullptr;
        }
      }

      else
      {
        assert(false && "Ctc: copy method not found");
        return nullptr;
      }
    }
};