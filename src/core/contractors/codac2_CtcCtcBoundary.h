/** 
 *  \file codac2_CtcCtcBoundary.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <functional>
#include "codac2_Interval.h"
#include "codac2_SlicedTube.h"
#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"
#include "codac2_Collection.h"
#include "codac2_BoolInterval.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class CtcCtcBoundary : public Ctc<CtcCtcBoundary,IntervalVector>
  {
    public:

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcCtcBoundary(const C& ctc_boundary, const std::function<BoolInterval(const Vector&)>& inside_test)
        : Ctc<CtcCtcBoundary,IntervalVector>(size_of(ctc_boundary)), _ctc_boundary(ctc_boundary), _inside_test(inside_test)
      { }

      void contract(IntervalVector& x) const;

    protected:

      const Collection<CtcBase<IntervalVector>> _ctc_boundary;
      const std::function<BoolInterval(const Vector&)> _inside_test;
  };
}