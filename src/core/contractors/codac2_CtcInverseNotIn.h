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
  class CtcInverseNotIn : public CtcUnion<X>
  {
    public:

      CtcInverseNotIn(const Function<Y>& f, const Y& y)
        : CtcUnion<X>(2/*std::dynamic_pointer_cast<const ArgBase>(f.args()[0])->size()*/)
      {
        assert(f.nb_args() == 1);

        for(const auto& complem_y : y.complementary())
          *this |= CtcInverse_<Y,X>(f, complem_y);
      }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcInverseNotIn<Y,X>>(*this);
      }
  };
}