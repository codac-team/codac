/** 
 *  \file
 *  Figure class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <string>
#include "codac2_IntervalVector.h"

namespace codac2
{
  class FigureInterface
  {
    public:

      virtual void draw_box(const IntervalVector& x, const std::string& color) = 0;
  };
}