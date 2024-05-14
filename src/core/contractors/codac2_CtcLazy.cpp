/** 
 *  CtcLazy class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_CtcLazy.h"

using namespace std;
using namespace codac2;

std::shared_ptr<Ctc> CtcLazy::copy() const
{
  return std::make_shared<CtcLazy>(*this);
}

void CtcLazy::contract(IntervalVector& x) const
{
  double v = x.volume();
  _ctc.front().contract(x);

  if(x.volume()/v > 1.5*_r)
    _ctc.front().contract(x);
}