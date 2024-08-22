/** 
 *  \file codac2_SepPolygon.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_SepCtcBoundary.h"
#include "codac2_CtcUnion.h"

namespace codac2
{
  class SepPolygon : public SepCtcBoundary
  {
    public:

      SepPolygon(const std::vector<IntervalVector>& vertices);
      std::shared_ptr<Sep> copy() const;
      BoxPair separate(const IntervalVector& x) const;
  };
}