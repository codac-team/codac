/** 
 *  \file codac2_SepCtcPair.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class SepCtcPair : public Sep<SepCtcPair>
  {
    public:

      template<typename C1, typename C2>
        requires (IsCtcBaseOrPtr<C1,IntervalVector> && IsCtcBaseOrPtr<C2,IntervalVector>)
      SepCtcPair(const C1& ctc_in, const C2& ctc_out)
        : Sep<SepCtcPair>(size_of(ctc_in)), _ctc_in_out(ctc_in, ctc_out)
      {
        assert_release(size_of(ctc_in) == size_of(ctc_out));
      }

      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<CtcBase<IntervalVector>> _ctc_in_out;
  };
}