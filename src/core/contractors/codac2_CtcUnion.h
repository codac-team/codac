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
  template<typename... X>
  class CtcUnion : public Ctc<CtcUnion<X...>,X...>
  {
    public:

      explicit CtcUnion(Index n)
        : Ctc<CtcUnion<X...>,X...>(n)
      {
        if constexpr(std::is_same_v<X...,Interval>)
          assert_release(n == 1);
      }

      template<typename C>
        requires (IsCtcBaseOrPtr<C,X...> && !std::is_same_v<CtcUnion<X...>,C>)
      CtcUnion(const C& c)
        : Ctc<CtcUnion<X...>,X...>(size_of(c)), _ctcs(c)
      { }

      template<typename... C>
        requires (IsCtcBaseOrPtr<C,X...> && ...)
      CtcUnion(const C&... c)
        : Ctc<CtcUnion<X...>,X...>(size_first_item(c...)), _ctcs(c...)
      {
        assert_release(all_same_size(c...));
      }

      template<typename X_> // single type
      void contract_impl(X_& x) const
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

      void contract(X&... x) const
      {
        // contract_impl(..) method for multiple types is not yet implemented
        contract_impl(x...);
      }

      template<typename C>
        requires std::is_base_of_v<CtcBase<X...>,C>
      CtcUnion<X...>& operator|=(const C& c)
      {
        assert_release(c.size() == this->size());
        _ctcs.push_back(c);
        return *this;
      }

      CtcUnion<X...>& operator|=(const std::shared_ptr<CtcBase<X...>>& c)
      {
        assert_release(c->size() == this->size());
        _ctcs.push_back(c);
        return *this;
      }

    protected:

      Collection<CtcBase<X...>> _ctcs;
  };

  template <>
  class CtcUnion<> : public CtcUnion<IntervalVector>
  { };

  template<typename Tuple>
  struct CtcUnionType;

  template<typename... T>
  struct CtcUnionType<std::tuple<T...>> {
    using Ctc = CtcUnion<T...>;
  };

  template<typename C1, typename C2>
  typename CtcUnionType<typename C1::ContractedTypes>::Ctc operator|(const C1& c1, const C2& c2)
  {
    return { c1, c2 };
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