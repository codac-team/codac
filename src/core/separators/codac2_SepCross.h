/** 
 *  \file codac2_SepCross.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_CtcCross.h"
#include "codac2_SepCtcPair.h"

namespace codac2
{
  class SepCross : public SepCtcPair
  {
    public:

      SepCross(const Segment& e, const IntervalVector &r) 
        : SepCtcPair(CtcCross(e, r), CtcNoCross(e, r))
      { }
  };
}