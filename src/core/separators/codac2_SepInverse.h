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
#include "codac2_Function.h"
#include "codac2_CtcInverse.h"
#include "codac2_CtcInverseNotIn.h"

namespace codac2
{
  template<typename Y>
  class SepInverse : public SepCtcPair
  {
    using X = IntervalVector;

    public:

      SepInverse(const Function<Y>& f, const Y& y)
        : SepCtcPair(CtcInverseNotIn<Y,X>(f,y), CtcInverse_<Y,X>(f,y))
      { }

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S>
        >::type>
      SepInverse(const Function<Y>& f, const S& sep_y)
        : SepCtcPair(CtcInverseNotIn<Y,X>(f,SepCtcIn(sep_y)), CtcInverse_<Y,X>(f,SepCtcOut(sep_y)))
      { }

      virtual std::shared_ptr<Sep> copy() const
      {
        return std::make_shared<SepInverse<Y>>(*this);
      }
  };

  template<typename Y>
  SepInverse<Y> inverse(const Function<Y>& f, const Y& y)
  {
    return SepInverse<Y>(f,y);
  }
}