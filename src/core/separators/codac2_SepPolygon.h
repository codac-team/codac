/** 
 *  \file codac2_SepPolygon.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Polygon.h"
#include "codac2_SepCtcBoundary.h"

namespace codac2
{
  class SepPolygon : public SepCtcBoundary
  {
    public:

      SepPolygon(const Polygon& p);
      BoxPair separate(const IntervalVector& x) const;
  };
}