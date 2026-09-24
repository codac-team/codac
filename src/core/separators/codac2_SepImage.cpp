/** 
 *  codac2_SepImage.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepImage.h"
#include "codac2_CtcPolar.h"

using namespace codac2;

BoxPair SepImage::separate(const IntervalVector& x) const
{
  return _sep.front()->separate(x);
}