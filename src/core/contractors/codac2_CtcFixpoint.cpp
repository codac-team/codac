/** 
 *  codac2_CtcFixpoint.cp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcFixpoint.h"

using namespace std;
using namespace codac2;

void CtcFixpoint::contract(IntervalVector& x) const
{
  double v;

  do {
    v = x.volume();
    _ctc.front().contract(x);
  } while(x.volume()/v < (1-_r));
}