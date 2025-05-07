/** 
 *  \file codac2_CtcCartProd.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Quentin Brateau, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_IntervalVector.h"
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"

namespace codac2
{
  class CtcCartProd : public Ctc<CtcCartProd,IntervalVector>
  {
    public:

      CtcCartProd(const Collection<CtcBase<IntervalVector>>& ctcs)
        : Ctc<CtcCartProd,IntervalVector>([ctcs] {
            Index n = 0;
            for(const auto& ci : ctcs)
              n += ci->size();
            return n;
        }()), _ctcs(ctcs)
      { }

      template<typename C>
        requires (IsCtcBaseOrPtr<C,IntervalVector> && !std::is_same_v<CtcCartProd,C>)
      CtcCartProd(const C& c)
        : Ctc<CtcCartProd,IntervalVector>(size_of(c)), _ctcs(c)
      { }

      template<typename... C>
        requires (IsCtcBaseOrPtr<C,IntervalVector> && ...)
      CtcCartProd(const C&... c)
        : Ctc<CtcCartProd,IntervalVector>((0 + ... + size_of(c))), _ctcs(c...)
      { }

      void contract(IntervalVector& x) const
      {
        assert_release(x.size() == this->size());

        Index i = 0;
        for(const auto& ci : _ctcs)
        {
          IntervalVector xi = x.subvector(i,i+ci->size()-1);
          ci->contract(xi);

          if(xi.is_empty())
          {
            x.set_empty();
            return;
          }
          
          x.put(i,xi);
          i += ci->size();
        }
        assert(i == x.size());
      }

    protected:

      Collection<CtcBase<IntervalVector>> _ctcs;
  };

  template<typename... C>
    requires ((is_ctc_v<C>) || ...) && (!(is_sep_v<C>) && ...)
  inline CtcCartProd cart_prod(const C&... c)
  {
    return CtcCartProd(c...);
  }
}