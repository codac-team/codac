/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCNOCROSS__
#define __CODAC2_CTCNOCROSS__

#include <codac2_Ctc.h>
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_Var.h>
#include <codac2_CtcCN.h>

namespace codac2
{
  /**
   * \class CtcNoCross
   */
  class CtcNoCross : public Ctc
  {
    public:

      CtcNoCross(const IntervalVector& w1, const IntervalVector& w2, const IntervalVector& m);
      void contract(IntervalVector& x);

      make_available_to_cn()

    protected:

      const IntervalVector _w1, _w2, _m;
      Var<IntervalVector> t, ab, mt, am, at, ta, tb;
      Var<Interval> z1, z2, k1, k2, k3, k4;
      ContractorNetwork _cn;
      CtcCN _ctc_cn;
  };
}

#endif