/** 
 *  codac2_CtcConstell.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcConstell.h"

using namespace std;
using namespace codac2;

CtcConstell::CtcConstell(const std::vector<IntervalVector>& M)
  : Ctc<CtcConstell,IntervalVector>(
      [&M]() { assert_release(!M.empty()); return M[0].size(); }()),
    _M(M)
{ }

void CtcConstell::contract(IntervalVector& x) const
{
  assert_release(x.size() == this->size());
  IntervalVector u = IntervalVector::empty(this->size());
  for(const auto& mi : _M)
    u |= x & mi;
  x = u;
}