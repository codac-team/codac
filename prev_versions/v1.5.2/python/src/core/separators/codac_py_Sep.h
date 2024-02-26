/** 
 *  \file
 *  Sep Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_PY_SEP_H__
#define __CODAC_PY_SEP_H__

#include <pybind11/pybind11.h>
#include "codac_type_caster.h"

#include <ibex_Sep.h>
#include <codac_IntervalVector.h>


namespace codac
{
  class pySep : public ibex::Sep
  {
    public:

      pySep(int n) : ibex::Sep(n)
      {

      }

      // Trampoline (need one for each virtual function)
      void separate(IntervalVector& xin, IntervalVector& xout) override
      {
        pybind11::gil_scoped_acquire gil;  // Acquire the GIL while in this scope

        // Try to look up the overloaded method on the Python side
        pybind11::function overload = pybind11::get_overload(this, "separate");

        if(overload) // method is found
        {
          auto obj = overload(xin, xout);  // calls the Python function
          if(pybind11::isinstance<pybind11::tuple>(obj))
          { 
            // Checks if it returned a Python integer type
            auto tup = obj.cast<pybind11::tuple>();
            if(tup.size() != 2)
            {
              std::cout << "Return type error: must return two IntervalVector objects, but get " << tup.size() << std::endl;
              std::cout << "The separate() method must return x_in and x_out." << std::endl << std::endl;
              return;
            }

            xin &= tup[0].cast<IntervalVector>();
            xout &= tup[1].cast<IntervalVector>();
            return; 
          }

          else
          {
            std::cout << "WARNING: Deprecated Python Separator." << std::endl;
            std::cout << "The separate() method must return two IntervalVector objects, to avoid unpredictable results." << std::endl;
            std::cout << "Following IBEX convention, it is assumed that x_in, x_out have been contracted by the function." << std::endl << std::endl;
            return;  // Python returned none, return false.
          }
        }

        return;
      }
  };
}

#endif