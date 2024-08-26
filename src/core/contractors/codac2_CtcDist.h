/** 
 *  \file codac2_CtcDist.h
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
  class CtcDist : public Ctc_<IntervalVector>
  {
    public:

      CtcDist()
        : Ctc_<IntervalVector>(5)
      { }

      std::shared_ptr<Ctc_<IntervalVector>> copy() const;
      void contract(IntervalVector& x) const;
      void contract(Interval& ax, Interval& ay, Interval& bx, Interval& by, Interval& d) const;
  };
}