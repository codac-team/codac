/** 
 *  \file
 *  Ctc Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_PY_CTC_H__
#define __CODAC_PY_CTC_H__

#include <pybind11/pybind11.h>
#include "codac_type_caster.h"

#include "codac_Ctc.h"
#include "codac_IntervalVector.h"


namespace codac
{
  class pyCtc : public Ctc
  {
    public:

      pyCtc(int n) : Ctc(n)
      {

      }

      // Trampoline (need one for each virtual function)
      void contract(IntervalVector& box) override
      {
        pybind11::gil_scoped_acquire gil; // Acquire the GIL while in this scope

        // Try to look up the overloaded method on the Python side
        pybind11::function overload = pybind11::get_overload(this, "contract");

        if(overload) // method is found
        {
          auto box_copy = IntervalVector(box);
          auto obj = overload(box_copy); // calls the Python function

          if(pybind11::isinstance<IntervalVector>(obj))
          {
            // Checks if it returned a Python integer type
            box &= obj.cast<IntervalVector>(); // casts it and assigns it to the value
            return;  // Return true; value should be used
          }

          else
          {
            box &= box_copy;
            std::cout << "WARNING: Deprecated Python Contractor." << std::endl;
            std::cout << "The contract() method must return an IntervalVector to avoid unpredictable results." << std::endl << std::endl;
            return ; // Python returned none, return false.
          }
        }
      }
  };
}

#endif