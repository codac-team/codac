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

SepCtcPair::SepCtcPair(std::shared_ptr<Ctc_<IntervalVector>> ctc_in, std::shared_ptr<Ctc_<IntervalVector>> ctc_out)
  : _ctc_in(ctc_in), _ctc_out(ctc_out)
{ }

std::shared_ptr<Sep> SepCtcPair::copy() const
{
  return std::make_shared<SepCtcPair>(
    std::dynamic_pointer_cast<Ctc_<IntervalVector>>(_ctc_in->copy()),
    std::dynamic_pointer_cast<Ctc_<IntervalVector>>(_ctc_out->copy()));
}

BoxPair SepCtcPair::separate(const IntervalVector& x) const
{
  IntervalVector x_in(x), x_out(x);
  _ctc_in->contract(x_in);
  _ctc_out->contract(x_out);
  return { x_in, x_out };
}