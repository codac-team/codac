/** 
 *  \file codac2_SepCartProd.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Quentin Brateau, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_SepWrapper.h"

namespace codac2
{
  class SepCartProd : public Sep<SepCartProd>
  {
    public:

      SepCartProd(const Collection<SepBase>& seps)
        : Sep<SepCartProd>([seps] {
            Index n = 0;
            for(const auto& si : seps)
              n += si->size();
            return n;
        }()), _seps(seps)
      { }

      template<typename S>
        requires (IsSepBaseOrPtr<S> && !std::is_same_v<SepCartProd,S>)
      SepCartProd(const S& s)
        : Sep<SepCartProd>(size_of(s)), _seps(s)
      { }

      template<typename... S>
        requires (IsSepBaseOrPtr<S> && ...)
      SepCartProd(const S&... s)
        : Sep<SepCartProd>((0 + ... + size_of(s))), _seps(s...)
      { }
      
      BoxPair separate(const IntervalVector& x) const;

    protected:

      Collection<SepBase> _seps;
  };

  template<typename... S>
    requires  ((is_sep_v<S>) || ...) && ((!is_ctc_v<S>) && ...)
  inline SepCartProd cart_prod(const S&... s)
  {
    return SepCartProd(s...);
  }
}