/** 
 *  \file
 *  SepCartProd class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Quentin Brateau, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_SepWrapper.h"

namespace codac2
{
  class SepCartProd : public Sep
  {
    public:

      template<typename S, typename = typename std::enable_if<(
          (std::is_base_of_v<Sep,S> && !std::is_same_v<SepCartProd,S>) || std::is_same_v<std::shared_ptr<Sep>,S>
        ), void>::type>
      SepCartProd(const S& s)
        : Sep(size_of(s)), _seps(s)
      { }

      template<typename... S, typename = typename std::enable_if<(true && ... && (
          (std::is_base_of_v<Sep,S> || std::is_same_v<std::shared_ptr<Sep>,S>)
        )), void>::type>
      SepCartProd(const S&... s)
        : Sep((0 + ... + size_of(s))), _seps(s...)
      { }
      
      std::shared_ptr<Sep> copy() const;
      BoxPair separate(const IntervalVector& x) const;

    protected:

      Collection<Sep> _seps;
  };

  template<typename S1, typename S2, typename = typename std::enable_if<(
      std::is_base_of_v<Sep,S1> &&
      std::is_base_of_v<Sep,S2>
    )>::type>
  inline SepCartProd cart_prod(const S1& s1, const S2& s2)
  {
    return SepCartProd(s1,s2);
  }
}