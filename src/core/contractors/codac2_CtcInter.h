/** 
 *  \file codac2_CtcInter.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
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
          (std::is_base_of_v<Ctc_<X>,C> && !std::is_same_v<CtcInter,C>) || std::is_same_v<std::shared_ptr<Ctc_<X>>,C>
        ), void>::type>
      CtcInter(const C& c)
        : Ctc_<X>(size_of(c)), _ctcs(c)
      { }

      template<typename... C, typename = typename std::enable_if<(true && ... && (
          (std::is_base_of_v<Ctc_<X>,C> || std::is_same_v<std::shared_ptr<Ctc_<X>>,C>)
        )), void>::type>
      CtcInter(const C&... c)
        : Ctc_<X>(size_first_item(c...)), _ctcs(c...)
      {
        assert(all_same_size(c...));
      }

      std::shared_ptr<Ctc> copy() const
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

  template<typename C1, typename C2, typename = typename std::enable_if<(
      std::is_base_of_v<Ctc_<typename C1::X>,C1> &&
      std::is_base_of_v<Ctc_<typename C1::X>,C2> &&
      std::is_same_v<typename C1::X,typename C2::X>
    )>>
  inline CtcInter<typename C1::X> operator&(const C1& c1, const C2& c2)
  {
    return CtcInter<typename C1::X>(c1,c2);
  }

  template<typename C2, typename = typename std::enable_if<
      std::is_base_of_v<Ctc_<IntervalVector>,C2>
    >::type>
  inline CtcInter<IntervalVector> operator&(const IntervalVector& s1, const C2& s2)
  {
    return CtcInter<IntervalVector>(CtcWrapper_<IntervalVector>(s1),s2);
  }

  template<typename C1, typename = typename std::enable_if<
      std::is_base_of_v<Ctc_<IntervalVector>,C1>
    >::type>
  inline CtcInter<IntervalVector> operator&(const C1& s1, const IntervalVector& s2)
  {
    return CtcInter<IntervalVector>(s1,CtcWrapper_<IntervalVector>(s2));
  }
}