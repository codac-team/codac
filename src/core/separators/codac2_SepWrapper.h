/** 
 *  \file
 *  SepWrapper class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_SEPWRAPPER__
#define __CODAC2_SEPWRAPPER__

#include "codac2_Sep.h"

namespace codac2
{
  template<typename T>
  class SepWrapper_ : public Sep
  {
    public:

      SepWrapper_(const T& y)
        : _y(y)
      { }

    protected:

      const T _y;
  };

  template<>
  class SepWrapper_<IntervalVector> : public Sep
  {
    public:

      SepWrapper_(const IntervalVector& y)
        : _y(y)
      { }

      BoxPair separate(const IntervalVector& x) const
      {
        IntervalVector x_in = x;
        x_in.set_empty();

        for(const auto& di : IntervalVector(x.size()).diff(_y))
          x_in |= x & di;

        return { x_in, x & _y };
      }

    protected:

      const IntervalVector _y;
  };
}

#endif