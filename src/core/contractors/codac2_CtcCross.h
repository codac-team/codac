/** 
 *  \file codac2_CtcCross.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"
#include "codac2_Segment.h"

namespace codac2
{
  class CtcCross : public Ctc<CtcCross,IntervalVector>
  {
    public:

      CtcCross(const Segment& e, const IntervalVector &r)
        : Ctc<CtcCross,IntervalVector>(2), _e(e), _r(r)
      {
        assert_release(_r.size() == 2 && "only 2d segments are supported");
      }

      void contract(IntervalVector& x) const;

    protected:

      const Segment _e;
      const IntervalVector _r;
  };

  class CtcNoCross : public Ctc<CtcNoCross,IntervalVector>
  {
    public:

      CtcNoCross(const Segment& e, const IntervalVector &r)
        : Ctc<CtcNoCross,IntervalVector>(2), _e(e), _r(r)
      {
        assert_release(_r.size() == 2 && "only 2d segments are supported");
      }

      void contract(IntervalVector& x) const;

    protected:

      const Segment _e;
      const IntervalVector _r;
  };
}