/** 
 *  \file
 *  SepInverse class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <map>
#include "codac2_Sep.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_CtcInverse.h"
#include "codac2_CtcInverseNotIn.h"
#include "codac2_SepCtc.h"

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

      template<typename S, typename = typename std::enable_if<
          std::is_same_v<Y,IntervalVector> && (std::is_base_of_v<Sep,S> || std::is_same_v<std::shared_ptr<Sep>,S>)
        >::type>
      SepInverse(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const S& sep_y, bool with_centered_form = true)
        : SepCtcPair(CtcInverseNotIn<Y,X>(f,SepCtcIn(sep_y),with_centered_form), CtcInverse_<Y,X>(f,SepCtcOut(sep_y),with_centered_form))
      { }

      virtual std::shared_ptr<Sep> copy() const
      {
        return std::make_shared<SepInverse<Y>>(*this);
      }
  };
}