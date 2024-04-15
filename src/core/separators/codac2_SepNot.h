/** 
 *  \file
 *  SepNot class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
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

      SepNot(const IntervalVector& s)
        : _sep(SepWrapper_<IntervalVector>(s))
      { }

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S>
        >::type>
      SepNot(const S& s)
        : _sep(s)
      { }

      virtual std::shared_ptr<Sep> copy() const
      {
        return std::make_shared<SepNot>(*this);
      }

      BoxPair separate(const IntervalVector& x) const
      {
        auto x_sep = _sep.front().separate(x);
        return { x_sep.out , x_sep.in };
      }

    protected:

      const Collection<Sep> _sep;
  };

  inline SepNot operator!(const IntervalVector& s)
  {
    return SepNot(s);
  }

  template<typename S, typename = typename std::enable_if<
      std::is_base_of_v<Sep,S>
    >::type>
  SepNot operator!(const S& s)
  {
    return SepNot(s);
  }
}