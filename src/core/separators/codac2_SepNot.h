/** 
 *  \file codac2_SepNot.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_SepWrapper.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class SepNot : public Sep<SepNot>
  {
    public:

      SepNot(const IntervalVector& x)
        : Sep<SepNot>(x.size()), _sep(SepWrapper_<IntervalVector>(x))
      { }

      template<typename S>
        requires (IsSepBaseOrPtr<S> && !std::is_same_v<SepNot,S>)
      SepNot(const S& s)
        : Sep<SepNot>(size_of(s)), _sep(s)
      { }

      BoxPair separate(const IntervalVector& x) const
      {
        assert_release(x.size() == this->size());
        
        auto x_sep = _sep.front().separate(x);
        assert((x_sep.outer | x_sep.inner) == x);
        return { x_sep.outer , x_sep.inner };
      }

    protected:

      const Collection<SepBase> _sep;
  };

  template<typename S>
    requires std::is_base_of_v<SepBase,S>
  SepNot operator!(const S& s)
  {
    return SepNot(s);
  }
}