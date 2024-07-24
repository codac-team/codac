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
      : Ctc_<IntervalVector>(n), _n(n)
    { }

    // Trampoline (need one for each virtual function)
    virtual void contract(IntervalVector& x) const override
    {
      pybind11::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      pybind11::function overload = pybind11::get_overload(this, "contract");

      if(overload) // method is found
        overload.operator()<
          pybind11::return_value_policy::reference // because contract() works by reference
        >(x);

      else
      {
        assert(false && "Ctc_<IntervalVector>: contract method not found");
      }
    }

    // Trampoline (need one for each virtual function)
    virtual std::shared_ptr<Ctc_<IntervalVector>> copy() const override
    {
      pybind11::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      pybind11::function overload = pybind11::get_overload(this, "copy");

      if(overload) // method is found
      {
        auto obj = overload();
        return std::shared_ptr<Ctc_<IntervalVector>>(obj.cast<Ctc_<IntervalVector>*>(), [](auto p) { /* no delete */ });
      }

      else
      {
        assert(false && "Ctc_<IntervalVector>: copy method not found");
        return nullptr;
      }
    }

  protected:

    const size_t_type _n;
};