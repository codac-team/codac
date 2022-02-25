/** 
 *  \file
 *  DynCtc Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_PY_DYNCTC_H__
#define __CODAC_PY_DYNCTC_H__

#include <pybind11/pybind11.h>
#include "codac_type_caster.h"

#include "codac_Domain.h"
#include "codac_DynCtc.h"


namespace codac
{
  class pyDynCtc : public DynCtc
  {
    public:

      // Trampoline (need one for each virtual function)
      void contract(std::vector<Domain*>& v_domains) override
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
}

#endif