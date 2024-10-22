/** 
 *  \file codac2_CtcIdentity.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_CtcWrapper.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  class CtcIdentity : public Ctc<CtcIdentity,IntervalVector>
  {
    public:

      CtcIdentity(size_t n);

      void contract(IntervalVector& x) const;
  };
}