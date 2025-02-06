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

namespace codac2
{
  // eps: accuracy of the paving algorithm, the undefined boxes will have their max_diam <= eps
  
  PavingOut pave(const IntervalVector& x, std::shared_ptr<const CtcBase<IntervalVector>> c, double eps, bool verbose = false);
  PavingOut pave(const IntervalVector& x, const CtcBase<IntervalVector>& c, double eps, bool verbose = false);
  PavingInOut pave(const IntervalVector& x, std::shared_ptr<const SepBase> s, double eps, bool verbose = false);
  PavingInOut pave(const IntervalVector& x, const SepBase& s, double eps, bool verbose = false);

  template<typename Y>
  PavingInOut sivia(const IntervalVector& x, const AnalyticFunction<Y>& f, const typename Y::Domain& y, double eps, bool verbose = false)
  {
    clock_t t_start = clock();

    PavingInOut p(x);
    std::list<std::shared_ptr<PavingInOut_Node>> l { p.tree() };

    while(!l.empty())
    {
      auto n = l.front();
      l.pop_front();

      assert(n->is_leaf());
      auto eval = f.eval(std::get<1>(n->boxes()));

      if(eval.is_subset(y))
        std::get<1>(n->boxes()).set_empty();

      else if(!eval.intersects(y))
        std::get<0>(n->boxes()).set_empty();

      else if(n->unknown().max_diam() > eps)
      {
        n->bisect();
        l.push_back(n->left());
        l.push_back(n->right());
      }
    }

    if(verbose)
      printf("Computation time: %.4fs\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);
    return p;
  }
}