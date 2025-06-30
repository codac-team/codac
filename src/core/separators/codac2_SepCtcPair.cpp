/** 
 *  codac2_SepCtcPair.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepCtcPair.h"

using namespace std;
using namespace codac2;

BoxPair SepCtcPair::separate(const IntervalVector& x) const
{
  assert(x.size() == this->size());

  IntervalVector x_in(x), x_out(x);
  _ctc_inner_outer.front()->contract(x_in);
  _ctc_inner_outer.back()->contract(x_out);
  
  assert((x_in | x_out) == x);
  return { x_in, x_out };
}