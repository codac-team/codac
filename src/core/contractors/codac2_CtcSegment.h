/** 
 *  \file codac2_CtcSegment.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  class CtcSegment : public Ctc<CtcSegment,IntervalVector>
  {
    public:

      CtcSegment(const IntervalVector& a, const IntervalVector& b)
        : Ctc<CtcSegment,IntervalVector>(2), _a(a), _b(b)
      {
        assert_release(a.size() == 2 && b.size() == 2 && "only 2d segments are supported");
      }

      void contract(IntervalVector& x) const;

    protected:

      const IntervalVector _a, _b;
  };
}