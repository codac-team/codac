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
using namespace py::literals;


#define CONTRACT_BOX_METHOD(clss,doc) \
  "contract", [](const clss& c, IntervalVector& x) -> const IntervalVector& { c.contract(x); return x; }, doc, "x"_a

class pyCtcIntervalVector : public Ctc_<IntervalVector>
{
  public:

    pyCtcIntervalVector(size_t_type n)
      : Ctc_<IntervalVector>(n), _n(n)
    { }

    // Trampoline (need one for each virtual function)
    virtual void contract(IntervalVector& x) const override
    {
      py::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      py::function overload = py::get_overload(this, "contract");
      assert_release(overload && "Ctc: contract method not found");

      auto obj = overload(x);
      assert_release(py::isinstance<IntervalVector>(obj) &&
        "Ctc: error with contract method, it should return an IntervalVector");

      IntervalVector contracted_x = obj.cast<IntervalVector>();
      x &= obj.cast<IntervalVector>();
    }

    // Trampoline (need one for each virtual function)
    virtual std::shared_ptr<Ctc_<IntervalVector>> copy() const override
    {
      py::gil_scoped_acquire gil; // Acquire the GIL while in this scope

      // Try to look up the overloaded method on the Python side
      py::function overload = py::get_overload(this, "copy");
      assert_release(overload && "Ctc_<IntervalVector>: copy method not found");

      auto obj = overload();
      return std::shared_ptr<Ctc_<IntervalVector>>(obj.cast<Ctc_<IntervalVector>*>(), [](auto p) { /* no delete */ });
    }

  protected:

    const size_t_type _n;
};