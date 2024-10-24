/** 
 *  \file codac2_CtcPolygon.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Polygon.h"
#include "codac2_CtcCtcBoundary.h"
#include "codac2_CtcUnion.h"

namespace codac2
{
  class CtcPolygon : public CtcCtcBoundary
  {
    public:

      CtcPolygon(const Polygon& p);
      void contract(IntervalVector& x) const;
  };
}