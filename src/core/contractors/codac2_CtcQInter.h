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

      explicit CtcQInter(unsigned int q, Index n, const Collection<CtcBase<IntervalVector>>& ctcs = {})
        : Ctc<CtcQInter,IntervalVector>(n), _q(q), _ctcs(ctcs)
      {
        assert_release(n > 0);
      }

      template<typename C>
        requires (IsCtcBaseOrPtr<C,IntervalVector> && !std::is_same_v<CtcQInter,C>)
      CtcQInter(unsigned int q, const C& c)
        : CtcQInter(q, size_of(c), {c})
      {
        assert_release(q <= 1);
      }

      template<typename... C>
        requires (IsCtcBaseOrPtr<C,IntervalVector> && ...)
      CtcQInter(unsigned int q, const C&... c)
        : CtcQInter(q, size_first_item(c...), {c...})
      {
        assert_release(all_same_size(c...));
        assert_release(q <= sizeof...(c));
      }

      size_t nb() const;

      void contract(IntervalVector& x) const;

    protected:

      size_t _q;
      Collection<CtcBase<IntervalVector>> _ctcs;
  };
}