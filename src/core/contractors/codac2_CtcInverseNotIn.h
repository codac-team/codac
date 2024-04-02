/** 
 *  \file
 *  CtcInverseNotIn class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <map>
#include "codac2_CtcUnion.h"
#include "codac2_CtcInverse.h"

namespace codac2
{
  template<typename Y,typename X>
  class CtcInverseNotIn : public Ctc_<X>
  {
    public:

      CtcInverseNotIn(const Function<Y>& f, const Y& y)
      {
        _ctc_not_in = CtcUnion<X>(); // empty initialization

        for(const auto& complem_y : y.complementary())
          _ctc_not_in |= CtcInverse_<Y,X>(f, complem_y);
      }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcInverseNotIn<Y,X>>(*this);
      }

      void contract(X& x) const
      {
        _ctc_not_in.contract(x);
      }

    protected:
      
      CtcUnion<X> _ctc_not_in;
  };
}