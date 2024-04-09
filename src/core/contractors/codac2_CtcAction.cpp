/** 
 *  CtcAction class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_CtcAction.h"

using namespace std;
using namespace codac2;

std::shared_ptr<Ctc> CtcAction::copy() const
{
  return std::make_shared<CtcAction>(*this);
}

void CtcAction::contract(IntervalVector& x) const
{
  IntervalVector _x(_s(x));
  _ctc.front().contract(_x);
  x &= __s(_x);
}