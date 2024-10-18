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

      template<typename C>
        requires (IsCtcBaseOrPtr<C,X> && !std::is_same_v<CtcInter<X>,C>)
      CtcInter(const C& c)
        : Ctc<CtcInter<X>,X>(size_of(c)), _ctcs(c)
      { }

      template<typename... C>
        requires (IsCtcBaseOrPtr<C,X> && ...)
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

      template<typename C>
        requires std::is_base_of_v<CtcBase<X>,C>
      CtcInter<X>& operator&=(const C& c)
      {
        assert_release(c.size() == this->size());
        _ctcs.add_shared_ptr(std::make_shared<C>(c));
        return *this;
      }

      CtcInter<X>& operator&=(const std::shared_ptr<CtcBase<X>>& c)
      {
        assert_release(c->size() == this->size());
        _ctcs.add_shared_ptr(c);
        return *this;
      }

    protected:

      Collection<CtcBase<X>> _ctcs;
  };

  template<typename C1, typename C2>
    requires (std::is_same_v<typename C1::ContractedType,typename C2::ContractedType>
      && std::is_base_of_v<CtcBase<typename C1::ContractedType>,C1>
      && std::is_base_of_v<CtcBase<typename C1::ContractedType>,C2>)
  inline CtcInter<typename C1::ContractedType> operator&(const C1& c1, const C2& c2)
  {
    return CtcInter<typename C1::ContractedType>(c1,c2);
  }

  template<typename C2>
    requires std::is_base_of_v<CtcBase<IntervalVector>,C2>
  inline CtcInter<IntervalVector> operator&(const IntervalVector& c1, const C2& c2)
  {
    assert_release(c1.size() == c2.size());
    return CtcInter<IntervalVector>(CtcWrapper_<IntervalVector>(c1),c2);
  }

  template<typename C1>
    requires std::is_base_of_v<CtcBase<IntervalVector>,C1>
  inline CtcInter<IntervalVector> operator&(const C1& c1, const IntervalVector& c2)
  {
    assert_release(c1.size() == c2.size());
    return CtcInter<IntervalVector>(c1,CtcWrapper_<IntervalVector>(c2));
  }
}