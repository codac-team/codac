/** 
 *  \file codac2_SepCtcBoundary.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <functional>
#include "codac2_Sep.h"
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_BoolInterval.h"

namespace codac2
{
  class SepCtcBoundary : public Sep
  {
    public:

      template<typename C, typename = typename std::enable_if<(
          std::is_base_of_v<Ctc_<IntervalVector>,C>
        )>::type>
      SepCtcBoundary(const C& ctc_boundary, const std::function<BoolInterval(const Vector&)>& inside_test)
        : Sep(ctc_boundary.size()), _ctc_boundary(ctc_boundary), _inside_test(inside_test)
      { }

      std::shared_ptr<Sep> copy() const;
      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<Ctc_<IntervalVector>> _ctc_boundary;
      const std::function<BoolInterval(const Vector&)> _inside_test;
  };
}