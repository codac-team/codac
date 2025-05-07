/** 
 *  \file codac2_CtcUnion.h
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
  template<typename X=IntervalVector>
  class CtcUnion : public Ctc<CtcUnion<X>,X>
  {
    public:

      explicit CtcUnion(Index n)
        : Ctc<CtcUnion<X>,X>(n)
      {
        if constexpr(std::is_same_v<X,Interval>)
        {
          assert(n == 1);
        }
      }

      template<typename C>
        requires (IsCtcBaseOrPtr<C,X> && !std::is_same_v<CtcUnion<X>,C>)
      CtcUnion(const C& c)
        : Ctc<CtcUnion<X>,X>(size_of(c)), _ctcs(c)
      { }

      template<typename... C>
        requires (IsCtcBaseOrPtr<C,X> && ...)
      CtcUnion(const C&... c)
        : Ctc<CtcUnion<X>,X>(size_first_item(c...)), _ctcs(c...)
      {
        assert_release(all_same_size(c...));
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

      template<typename C>
        requires std::is_base_of_v<CtcBase<X>,C>
      CtcUnion<X>& operator|=(const C& c)
      {
        assert_release(c.size() == this->size());
        _ctcs.push_object_back(c);
        return *this;
      }

      CtcUnion<X>& operator|=(const std::shared_ptr<CtcBase<X>>& c)
      {
        assert_release(c->size() == this->size());
        _ctcs.push_back(c);
        return *this;
      }

    protected:

      Collection<CtcBase<X>> _ctcs;
  };

  template<typename C1, typename C2>
    requires (std::is_same_v<typename C1::ContractedType,typename C2::ContractedType>
      && std::is_base_of_v<CtcBase<typename C1::ContractedType>,C1>
      && std::is_base_of_v<CtcBase<typename C1::ContractedType>,C2>)
  inline CtcUnion<typename C1::ContractedType> operator|(const C1& c1, const C2& c2)
  {
    return CtcUnion<typename C1::ContractedType>(c1,c2);
  }

  template<typename C2>
    requires std::is_base_of_v<CtcBase<IntervalVector>,C2>
  inline CtcUnion<IntervalVector> operator|(const IntervalVector& c1, const C2& c2)
  {
    assert_release(c1.size() == c2.size());
    return CtcUnion<IntervalVector>(CtcWrapper(c1),c2);
  }

  template<typename C1>
    requires std::is_base_of_v<CtcBase<IntervalVector>,C1>
  inline CtcUnion<IntervalVector> operator|(const C1& c1, const IntervalVector& c2)
  {
    assert_release(c1.size() == c2.size());
    return CtcUnion<IntervalVector>(c1,CtcWrapper(c2));
  }
}