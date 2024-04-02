/** 
 *  SepWrapper class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_SepWrapper.h"

using namespace std;
using namespace codac2;

std::shared_ptr<Sep> SepWrapper_<IntervalVector>::copy() const
{
  return std::make_shared<SepWrapper_<IntervalVector>>(_y);
}

BoxPair SepWrapper_<IntervalVector>::separate(const IntervalVector& x) const
{
  return SepCtcPair::separate(x);
}