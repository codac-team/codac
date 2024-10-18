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

BoxPair SepWrapper_<IntervalVector>::separate(const IntervalVector& x) const
{
  return SepCtcPair::separate(x);
}