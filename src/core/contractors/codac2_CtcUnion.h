/** 
 *  \file
 *  CtcUnion class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"

namespace codac2
{
  template<typename X>
  class CtcUnion : public Ctc_<X>
  {
    public:

      explicit CtcUnion(size_t n)
        : Ctc_<X>(n)
      { }

      template<typename C1, typename... C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<X>,C1> && 
          (true && ... && (std::is_base_of_v<Ctc_<X>,C>)), void
        >::type>
      CtcUnion(const C1& c1, const C&... c)
        : Ctc_<X>(c1.size()), _ctcs(c...)
      {
        _ctcs.add_shared_ptr(std::make_shared<C1>(c1));
      }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcUnion<X>>(*this);
      }

      void contract(X& x) const
      {
        auto result = x;
        result.set_empty();

        for(const auto& ci : _ctcs)
        {
          auto saved_x = x;
          ci->contract(saved_x);
          result |= saved_x;
        }

        x = result;
      }

      template<typename C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<X>,C>
        >::type>
      CtcUnion<X>& operator|=(const C& c)
      {
        _ctcs.add_shared_ptr(std::make_shared<C>(c));
        return *this;
      }

    protected:

      Collection<Ctc_<X>> _ctcs;
  };

  template<typename C1, typename C2, typename = typename std::enable_if<(
      std::is_base_of_v<Ctc_<typename C1::X>,C1> &&
      std::is_base_of_v<Ctc_<typename C1::X>,C2> &&
      std::is_same_v<typename C1::X,typename C2::X>
    )>>
  inline CtcUnion<typename C1::X> operator|(const C1& c1, const C2& c2)
  {
    return CtcUnion<typename C1::X>(c1,c2);
  }
}