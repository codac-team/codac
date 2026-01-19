/** 
 *  \file codac2_SepQInter.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_SepWrapper.h"

namespace codac2
{
  class SepQInter : public Sep<SepQInter>
  {
    public:

      explicit SepQInter(unsigned int q, Index n, const Collection<SepBase>& sep = {})
        : Sep<SepQInter>(n), _q(q), _seps(sep)
      {
        assert_release(n > 0);
      }

      template<typename S>
        requires (IsSepBaseOrPtr<S> && !std::is_same_v<SepQInter,S>)
      SepQInter(unsigned int q, const S& s)
        : SepQInter(q, size_of(s), {s})
      {
        assert_release(q <= 1);
      }

      template<typename... S>
        requires (IsSepBaseOrPtr<S> && ...)
      SepQInter(unsigned int q, const S&... s)
        : SepQInter(q, size_first_item(s...), {s...})
      {
        assert_release(all_same_size(s...));
        assert_release(q <= sizeof...(s));
      }

      size_t nb() const;

      BoxPair separate(const IntervalVector& x) const;

    protected:

      size_t _q;
      Collection<SepBase> _seps;
  };
}