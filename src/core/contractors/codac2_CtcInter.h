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
#include "codac2_template_tools.h"

namespace codac2
{
  template<typename X>
  class CtcInter : public Ctc<CtcInter<X>,X>
  {
    public:

      template<typename C, typename = typename std::enable_if<(
          (std::is_base_of_v<Ctc_<X>,C> && !std::is_same_v<CtcInter,C>) || std::is_same_v<std::shared_ptr<Ctc_<X>>,C>
        ), void>::type>
      CtcInter(const C& c)
        : Ctc<CtcInter<X>,X>(size_of(c)), _ctcs(c)
      { }

      template<typename... C, typename = typename std::enable_if<(true && ... && (
          (std::is_base_of_v<Ctc_<X>,C> || std::is_same_v<std::shared_ptr<Ctc_<X>>,C>)
        )), void>::type>
      CtcInter(const C&... c)
        : Ctc<CtcInter<X>,X>(size_first_item(c...)), _ctcs(c...)
      {
        assert_release(all_same_size(c...));
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
        assert_release(c.size() == this->size());
        _ctcs.add_shared_ptr(std::make_shared<C>(c));
        return *this;
      }

      CtcInter<X>& operator&=(const std::shared_ptr<Ctc_<X>>& c)
      {
        assert_release(c->size() == this->size());
        _ctcs.add_shared_ptr(c);
        return *this;
      }

    protected:

      Collection<Ctc_<X>> _ctcs;
  };

  template<typename C1, typename C2, typename = typename std::enable_if<(
      std::is_base_of_v<Ctc_<typename C1::ContractedType>,C1> &&
      std::is_base_of_v<Ctc_<typename C1::ContractedType>,C2> &&
      std::is_same_v<typename C1::ContractedType,typename C2::ContractedType>
    )>>
  inline CtcInter<typename C1::ContractedType> operator&(const C1& c1, const C2& c2)
  {
    return CtcInter<typename C1::ContractedType>(c1,c2);
  }

  template<typename C2, typename = typename std::enable_if<
      std::is_base_of_v<Ctc_<IntervalVector>,C2>
    >::type>
  inline CtcInter<IntervalVector> operator&(const IntervalVector& c1, const C2& c2)
  {
    assert_release(c1.size() == c2.size());
    return CtcInter<IntervalVector>(CtcWrapper_<IntervalVector>(c1),c2);
  }

  template<typename C1, typename = typename std::enable_if<
      std::is_base_of_v<Ctc_<IntervalVector>,C1>
    >::type>
  inline CtcInter<IntervalVector> operator&(const C1& c1, const IntervalVector& c2)
  {
    assert_release(c1.size() == c2.size());
    return CtcInter<IntervalVector>(c1,CtcWrapper_<IntervalVector>(c2));
  }
}