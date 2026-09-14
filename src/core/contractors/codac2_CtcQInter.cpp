/** 
 *  codac2_CtcQInter.cp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcQInter.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  size_t CtcQInter::nb() const
  {
    return _ctcs.size();
  }

  void CtcQInter::contract(IntervalVector& x) const
  {
    // If all constraints may be violated, no contraction can be performed.
    if(_q == _ctcs.size())
      return;
    
    std::list<IntervalVector> l;

    for(const auto& ci : _ctcs)
    {
      IntervalVector x_(x);
      ci->contract(x_);
      l.push_back(x_);
    }

    x = qinter(_q, l);
  }
}
