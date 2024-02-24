/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCDIST__
#define __CODAC2_CTCDIST__

#include "codac2_Ctc.h"
#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  /**
   * \class CtcDist
   */
  class CtcDist : public Ctc
  {
    public:

      CtcDist();
      void contract(IntervalVector& x);
      void contract(IntervalVector& a, IntervalVector& b, Interval& d);
      void contract(Interval& ax, Interval& ay, Interval& bx, Interval& by, Interval& d);

      make_available_to_cn()
  };
}

#endif