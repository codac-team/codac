/** 
 *  \file codac2_SepInverse.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_Sep.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_CtcInverse.h"
#include "codac2_CtcInverseNotIn.h"
#include "codac2_CtcInnerOuter.h"
#include "codac2_SepCtcPair.h"
#include "codac2_template_tools.h"

namespace codac2
{
  template<typename Y>
  class SepInverse : public SepCtcPair
  {
    using X = IntervalVector;

    public:

      SepInverse(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Y& y, bool with_centered_form = true)
        : SepCtcPair(CtcInverseNotIn<Y,X>(f,y,with_centered_form), CtcInverse_<Y,X>(f,y,with_centered_form))
      { }

      template<typename S>
        requires (std::is_same_v<IntervalVector,Y> && IsSepBaseOrPtr<S>)
      SepInverse(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const S& sep_y, bool with_centered_form = true)
        : SepCtcPair(CtcInverseNotIn<Y,X>(f,CtcInner(sep_y),with_centered_form), CtcInverse_<Y,X>(f,CtcOuter(sep_y),with_centered_form))
      { }
  };
}