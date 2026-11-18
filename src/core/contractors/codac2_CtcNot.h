/** 
 *  \file codac2_CtcNot.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
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

      void contract([[maybe_unused]] IntervalVector& x) const
      {
        /* nothing can be done: no inner information */
      }
  };
}