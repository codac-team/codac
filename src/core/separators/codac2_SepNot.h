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
#include <map>
#include "codac2_Sep.h"

namespace codac2
{
  class SepNot : public CollectionSep, public Sep
  {
    public:

      SepNot(const IntervalVector& s)
        : CollectionSep(SepWrapper_<IntervalVector>(s))
      { }

      template<typename S, // S should be some Sep class
        typename = typename std::enable_if<std::is_base_of<Sep,S>::value>::type>
      SepNot(const S& s)
        : CollectionSep(s)
      { }

      virtual std::shared_ptr<Sep> copy() const
      {
        return std::make_shared<SepNot>(*this);
      }

      BoxPair separate(const IntervalVector& x) const
      {
        auto x_sep = _v_sep_ptrs.front()->separate(x);
        return { x_sep.out , x_sep.in };
      }
  };
}