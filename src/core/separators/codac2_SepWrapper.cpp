/** 
 *  codac2_SepWrapper.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepWrapper.h"

using namespace std;
using namespace codac2;

std::shared_ptr<Sep> SepWrapper_<IntervalVector>::copy() const
{
  return std::make_shared<SepWrapper_<IntervalVector>>(*this);
}

BoxPair SepWrapper_<IntervalVector>::separate(const IntervalVector& x) const
{
  return SepCtcPair::separate(x);
}