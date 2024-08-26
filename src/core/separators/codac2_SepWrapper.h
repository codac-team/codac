/** 
 *  \file codac2_SepWrapper.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_SepCtcPair.h"
#include "codac2_CtcWrapper.h"
#include "codac2_CtcUnion.h"

namespace codac2
{
  template<typename T>
  class SepWrapper_ : public Sep<SepWrapper_<T>>
  {
    public:

      SepWrapper_(const T& y)
        : Sep<SepWrapper_<T>>(y.size()), _y(y)
      { }

    protected:

      const T _y;
  };

  template<>
  class SepWrapper_<IntervalVector> : public SepCtcPair
  {
    public:

      SepWrapper_(const IntervalVector& y)
        : SepCtcPair(complementary_union(y), CtcWrapper_<IntervalVector>(y))
      { }

      BoxPair separate(const IntervalVector& x) const;

    protected:

      CtcUnion<IntervalVector> complementary_union(const IntervalVector& y) const
      {
        CtcUnion<IntervalVector> cu(y.size());
        for(const auto& complem_y : y.complementary())
          cu |= CtcWrapper_<IntervalVector>(complem_y);
        return cu;
      }
  };
}