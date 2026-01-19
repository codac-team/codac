/** 
 *  \file codac2_pave.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Paving.h"
#include "codac2_Ctc.h"
#include "codac2_Sep.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_BoolInterval.h"

namespace codac2
{
  // eps: accuracy of the paving algorithm, the undefined boxes will have their max_diam <= eps
  
  PavingOut pave(const IntervalVector& x, std::shared_ptr<const CtcBase<IntervalVector>> c, double eps, bool verbose = false);
  PavingOut pave(const IntervalVector& x, const CtcBase<IntervalVector>& c, double eps, double& time, bool verbose = false);
  PavingOut pave(const IntervalVector& x, const CtcBase<IntervalVector>& c, double eps, bool verbose = false);

  PavingInOut pave(const IntervalVector& x, std::shared_ptr<const SepBase> s, double eps, bool verbose = false);
  PavingInOut pave(const IntervalVector& x, const SepBase& s, double eps, bool verbose = false);

  PavingInOut regular_pave(const IntervalVector& x, const std::function<BoolInterval(const IntervalVector&)>& test, double eps, bool verbose = false);

  template<typename Y>
  PavingInOut sivia(const IntervalVector& x, const AnalyticFunction<Y>& f, const typename Y::Domain& y, double eps, bool verbose = false)
  {
    return regular_pave(x,
      [&y,&f](const IntervalVector& x)
      {
        auto eval = f.eval(x);

        if(eval.is_subset(y))
          return BoolInterval::TRUE;

        else if(!eval.intersects(y))
          return BoolInterval::FALSE;

        else
          return BoolInterval::UNKNOWN;
      },
      eps, verbose);
  }
}