/** 
 *  \file
 *  Ctc Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_PY_CTC_H__
#define __TUBEX_PY_CTC_H__

#include <pybind11/pybind11.h>
#include "pyIbex_type_caster.h"

#include "ibex_Ctc.h"

using namespace std;
using namespace ibex;


class pyCtc : public Ctc
{
  public:

    pyCtc(int v): Ctc(v) {}

    // Trampoline (need one for each virtual function)
    void contract(IntervalVector& box) override
    {
      PYBIND11_OVERLOAD_PURE
      (
        void,     // return type
        Ctc,      // Parent class
        contract, // Name of function
        box       // Argument(s)
      );
    }
};

#endif