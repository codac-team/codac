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

#pragma once

#include "codac2_Sep.h"
#include "codac2_SepCtcPair.h"
#include "codac2_CtcWrapper.h"
#include "codac2_CtcUnion.h"

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
  class SepWrapper_<IntervalVector> : public SepCtcPair
  {
    public:

      SepWrapper_(const IntervalVector& y)
        : SepCtcPair(CtcUnion<IntervalVector>(y.size()),CtcWrapper_<IntervalVector>(y))
      {
        for(const auto& complem_y : y.complementary())
          dynamic_cast<CtcUnion<IntervalVector>&>(_ctc_in_out.front()) |= CtcWrapper_<IntervalVector>(complem_y);
      }

      virtual std::shared_ptr<Sep> copy() const;
      BoxPair separate(const IntervalVector& x) const;
  };
}