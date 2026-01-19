/** 
 *  codac2_SepQInter.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepQInter.h"
#include "codac2_qinter.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  size_t SepQInter::nb() const
  {
    return _seps.size();
  }

  BoxPair SepQInter::separate(const IntervalVector& x) const
  {
    if(_q == 0)
      return { IntervalVector::empty(x.size()), x };

    std::list<IntervalVector> l_inner, l_outer;
    for(const auto& si : _seps)
    {
      auto x_sep = si->separate(x);
      l_inner.push_back(x_sep.inner);
      l_outer.push_back(x_sep.outer);
    }

    BoxPair x_sep {
      qinter(_seps.size()-_q+1, l_inner),
      qinter(_q, l_outer)
    };

    assert((x_sep.inner | x_sep.outer) == x);
    return x_sep;
  }
}