/** 
 *  \file codac2_CtcNot.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_CtcUnion.h"
#include "codac2_CtcInverse.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class CtcNot : public Ctc<CtcNot,IntervalVector>
  {
    public:

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcNot(const C& c)
        : Ctc<CtcNot,IntervalVector>(size_of(c))
      { }

      void contract(IntervalVector& x) const
      {
        /* does nothing: no inner information */
      }
  };
}