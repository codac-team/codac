/** 
 *  \file codac2_SepCtcBoundary.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <functional>
#include "codac2_Sep.h"
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_BoolInterval.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class SepCtcBoundary : public Sep<SepCtcBoundary>
  {
    public:

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      SepCtcBoundary(const C& ctc_boundary, const std::function<BoolInterval(const Vector&)>& inside_test)
        : Sep<SepCtcBoundary>(size_of(ctc_boundary)), _ctc_boundary(ctc_boundary), _inside_test(inside_test)
      { }

      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<CtcBase<IntervalVector>> _ctc_boundary;
      const std::function<BoolInterval(const Vector&)> _inside_test;
  };
}