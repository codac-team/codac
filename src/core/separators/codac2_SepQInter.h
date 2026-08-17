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

      SepQInter(unsigned int q, const Collection<SepBase>& c)
        : Sep<SepQInter>([&c]() {
            assert_release(!c.empty());
            Index n = size_of(c.front());
            for(const auto& s : c) {
              assert_release(size_of(s) == n && "all separators must be of same size");
            }
            return n;
          }()), _q(q), _seps(c)
      {
        assert_release(q <= c.size());
      }

      explicit SepQInter(unsigned int q, Index n, const Collection<SepBase>& sep = {})
        : Sep<SepQInter>(n), _q(q), _seps(sep)
      {
        std::cout << "SepQInter::SepQInter(unsigned int q, Index n, ..) is deprecated." << std::endl;
        std::cout << "Use SepQInter::SepQInter(unsigned int q, ..) instead." << std::endl;
        assert_release(n > 0);
      }

      template<typename S>
        requires (IsSepBaseOrPtr<S> && !std::is_same_v<SepQInter,S>)
      SepQInter(unsigned int q, const S& s)
        : SepQInter(q, Collection<SepBase>({s}))
      { }

      template<typename... S>
        requires (IsSepBaseOrPtr<S> && ...)
      SepQInter(unsigned int q, const S&... s)
        : SepQInter(q, Collection<SepBase>({s...}))
      { }

      size_t nb() const;

      BoxPair separate(const IntervalVector& x) const;

    protected:

      size_t _q;
      Collection<SepBase> _seps;
  };
}