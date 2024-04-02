/** 
 *  \file
 *  CtcInter class
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
  class CtcInter : public Ctc_<X>
  {
    public:

      template<typename C, typename = typename std::enable_if<(
          std::is_base_of_v<Ctc_<X>,C> &&
          !std::is_same_v<CtcInter,C>
        ), void>::type>
      CtcInter(const C& c)
        : _ctcs(c)
      { }

      template<typename... C, typename = typename std::enable_if<(true && ... && (
          std::is_base_of_v<Ctc_<X>,C>
        )), void>::type>
      CtcInter(const C&... c)
        : _ctcs(c...)
      { }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcInter<X>>(*this);
      }

      void contract(X& x) const
      {
        for(const auto& ci : _ctcs)
          ci->contract(x);
      }

      template<typename C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<X>,C>
        >::type>
      CtcInter<X>& operator&=(const C& c)
      {
        _ctcs.add_shared_ptr(std::make_shared<C>(c));
        return *this;
      }

    protected:

      Collection<Ctc_<X>> _ctcs;
  };

  template<typename X, typename C1, typename C2, typename = typename std::enable_if<(
      std::is_base_of_v<Ctc_<X>,C1> &&
      std::is_base_of_v<Ctc_<X>,C2>
    )>>
  inline CtcInter<X> operator&(const C1& c1, const C2& c2)
  {
    return CtcInter<X>(c1,c2);
  }
}