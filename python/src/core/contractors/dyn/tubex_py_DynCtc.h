/** 
 *  \file
 *  DynCtc Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_PY_DYNCTC_H__
#define __TUBEX_PY_DYNCTC_H__

#include <pybind11/pybind11.h>
#include "pyIbex_type_caster.h"

#include "tubex_Domain.h"
#include "tubex_DynCtc.h"

using namespace std;
using namespace tubex;


class pyDynCtc : public DynCtc
{
  public:

    using DynCtc::DynCtc;

    // Trampoline (need one for each virtual function)
    void contract(vector<Domain*>& v_domains) override
    {
      PYBIND11_OVERLOAD_PURE
      (
        void,     // return type
        DynCtc,   // Parent class
        contract, // Name of function
        v_domains // Argument(s)
      );
    }
};


#endif