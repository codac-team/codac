/** 
 *  \file codac2_CtcWrapper.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"

namespace codac2
{
  template<typename Y,
   typename X = std::conditional_t<
      std::is_same_v<Y, Interval>,
      Interval,
      IntervalVector>>
  class CtcWrapper : public Ctc<CtcWrapper<Y>,X>
  {
    public:

      CtcWrapper(const Y& y)
        : Ctc<CtcWrapper<Y>,X>(y.size()), _y(y)
      { }

      void contract(X& x) const
      {
        assert_release(x.size() == this->size());
        x = _y & x;
      }

    protected:

      const Y _y;
  };
}