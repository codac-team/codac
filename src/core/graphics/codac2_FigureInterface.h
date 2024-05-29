/** 
 *  \file codac2_FigureInterface.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include "codac2_IntervalVector.h"
#include "codac2_StyleProperties.h"

namespace codac2
{
  class FigureInterface
  {
    public:

      virtual void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties()) = 0;
  };
}