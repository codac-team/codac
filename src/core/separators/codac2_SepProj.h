/** 
 *  \file
 *  SepProj class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_Sep.h"

namespace codac2
{
  class SepProj : public Sep
  {
    public:

      SepProj(const Sep& s, const IntervalVector& y);
      BoxPair separate(const IntervalVector& x, double eps) const;

    protected:

      const Sep& _s;
      const IntervalVector _y;
  };
}