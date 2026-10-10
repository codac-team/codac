/** 
 *  \file codac2_CtcQInter.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_qinter.h"
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class CtcQInter : public Ctc<CtcQInter,IntervalVector>
  {
    public:

      // q is the maximum number of constraints that may be violated.
      CtcQInter(unsigned int q, const Collection<CtcBase<IntervalVector>>& c)
        : Ctc<CtcQInter,IntervalVector>([&c]() {
            assert_release(!c.empty());
            Index n = size_of(c.front());
            for(const auto& s : c) {
              (void)s;
              assert_release(size_of(s) == n && "all contractors must be of same size");
            }
            return n;
          }()), _q(q), _ctcs(c)
      {
        assert_release(q <= c.size());
      }

      // q is the maximum number of constraints that may be violated.
      explicit CtcQInter(unsigned int q, Index n, const Collection<CtcBase<IntervalVector>>& ctcs = {})
        : Ctc<CtcQInter,IntervalVector>(n), _q(q), _ctcs(ctcs)
      {
        std::cout << "CtcQInter::CtcQInter(unsigned int q, Index n, ..) is deprecated." << std::endl;
        std::cout << "Use CtcQInter::CtcQInter(unsigned int q, ..) instead." << std::endl;
        assert_release(n > 0);
        assert_release(q <= ctcs.size());
      }

      template<typename C>
        requires (IsCtcBaseOrPtr<C,IntervalVector> && !std::is_same_v<CtcQInter,C>)
      CtcQInter(unsigned int q, const C& c)
        : CtcQInter(q, {c})
      { }

      template<typename... C>
        requires (IsCtcBaseOrPtr<C,IntervalVector> && ...)
      CtcQInter(unsigned int q, const C&... c)
        : CtcQInter(q, {c...})
      { }

      size_t nb() const;

      void contract(IntervalVector& x) const;

    protected:

      size_t _q;
      Collection<CtcBase<IntervalVector>> _ctcs;
  };
}
