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

      CtcInverseNotIn(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Y& y)
        : CtcUnion<X>(f.args()[0]->size() /* f must have only one arg, see following assert */)
      {
        assert(f.args().size() == 1);

        for(const auto& complem_y : y.complementary())
          *this |= CtcInverse_<Y,X>(f, complem_y);
      }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcInverseNotIn<Y,X>>(*this);
      }
  };
}