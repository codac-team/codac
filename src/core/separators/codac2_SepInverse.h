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

namespace codac2
{
  class SepInverse : public SepCtcPair
  {
    using X = IntervalVector;

    public:

      template<typename T>
      SepInverse(const AnalyticFunction<T>& f, const typename T::Domain& y, bool with_centered_form = true)
        : SepCtcPair(
            CtcInverseNotIn<typename T::Domain,X>(f,y,with_centered_form),
            CtcInverse<typename T::Domain,X>(f,y,with_centered_form))
      { }

      template<typename T,typename S>
        requires (std::is_same_v<T,VectorType> && IsSepBaseOrPtr<S>)
      SepInverse(const AnalyticFunction<T>& f, const S& sep_y, bool with_centered_form = true)
        : SepCtcPair(
            CtcInverseNotIn<typename T::Domain,X>(f,CtcInner(sep_y),with_centered_form),
            CtcInverse<typename T::Domain,X>(f,CtcOuter(sep_y),with_centered_form))
      { }
  };
}