/** 
 *  codac2_CtcLazy.cp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcLazy.h"

using namespace std;
using namespace codac2;

void CtcLazy::contract(IntervalVector& x) const
{
  double v = x.volume();
  _ctc.front().contract(x);

  if(x.volume()/v < 1.1*_r)
    _ctc.front().contract(x);
}