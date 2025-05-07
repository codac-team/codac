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
  template<typename X>
  class CtcWrapper : public Ctc<CtcWrapper<X>,X>
  {
    public:

      CtcWrapper(const X& y)
        : Ctc<CtcWrapper<X>,X>(y.size()), _y(y)
      { }

      void contract(X& x) const
      {
        assert_release(x.size() == this->size());
        x &= _y;
      }

    protected:

      const X _y;
  };
}