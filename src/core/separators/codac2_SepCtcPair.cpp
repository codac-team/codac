/** 
 *  SepCtcPair class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_SepCtcPair.h"

using namespace std;
using namespace codac2;

std::shared_ptr<Sep> SepCtcPair::copy() const
{
  return std::make_shared<SepCtcPair>(*this);
}

BoxPair SepCtcPair::separate(const IntervalVector& x) const
{
  IntervalVector x_in(x), x_out(x);
  _ctc_in_out.front().contract(x_in);
  _ctc_in_out.back().contract(x_out);
  return { x_in, x_out };
}