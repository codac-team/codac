/** 
 *  \file codac2_SepNot.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_SepWrapper.h"
#include "codac2_Collection.h"

namespace codac2
{
  class SepNot : public Sep
  {
    public:

      SepNot(const IntervalVector& x)
        : Sep(x.size()), _sep(SepWrapper_<IntervalVector>(x))
      { }

      template<typename S, typename = typename std::enable_if<(
          (std::is_base_of_v<Sep,S> && !std::is_same_v<SepNot,S>) || std::is_same_v<std::shared_ptr<Sep>,S>
        ), void>::type>
      SepNot(const S& s)
        : Sep(size_of(s)), _sep(s)
      { }

      std::shared_ptr<Sep> copy() const
      {
        return std::make_shared<SepNot>(*this);
      }

      BoxPair separate(const IntervalVector& x) const
      {
        assert_release(x.size() == this->size());
        
        auto x_sep = _sep.front().separate(x);
        assert((x_sep.outer | x_sep.inner) == x);
        return { x_sep.outer , x_sep.inner };
      }

    protected:

      const Collection<Sep> _sep;
  };

  template<typename S, typename = typename std::enable_if<
      std::is_base_of_v<Sep,S>
    >::type>
  SepNot operator!(const S& s)
  {
    return SepNot(s);
  }
}