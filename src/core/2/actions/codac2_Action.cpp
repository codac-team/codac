/** 
 *  OctaSym class
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_Action.h"
#include "codac2_CtcAction.h"

using namespace std;
using namespace codac;

namespace codac2
{
  OctaSym::OctaSym(const std::vector<int>& s) : std::vector<int>(s)
  {
    for(const auto& i : s)
    {
      assert((size_t)abs(i) > 0 && (size_t)abs(i) <= size());
    }
  }

  IntervalVector OctaSym::operator()(const IntervalVector& x) const
  {
    assert((size_t)x.size() == size());
    IntervalVector xs(size());
    for(size_t i = 0 ; i < size() ; i++)
      xs[i] = sign((*this)[i])*x[abs((*this)[i])-1];
    return xs;
  }

  OctaSym OctaSym::invert() const
  {
    vector<int> inv(size());
    for(size_t i = 0 ; i < size() ; i++)
      inv[abs((int)(*this)[i])-1] = (abs((int)i)+1)*((*this)[i] >= 0 ? 1. : -1.);
    return OctaSym(inv);
  }

  CtcAction OctaSym::operator()(Ctc& ctc) const
  {
    return CtcAction(ctc, *this);
  }
}