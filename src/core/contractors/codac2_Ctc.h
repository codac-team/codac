/** 
 *  \file codac2_Ctc.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include <iostream>
#include "codac2_Index.h"
#include "codac2_assert.h"
#include "codac2_TypeInfo.h"
#include "codac2_IntervalVector.h"
#include "codac2_SlicedTube.h"

namespace codac2
{
  template<typename... X>
  class CtcBase
  {
    public:
      
      using ContractedTypes = std::tuple<X...>;

      CtcBase(Index n)
        : _n(n)
      {
        assert(n > 0);
      }

      virtual ~CtcBase() = default;

      Index size() const
      {
        return _n;
      }
      
      virtual void contract(X&... x) const = 0;

      virtual void contract_tube(SlicedTube<X>&... x) const
      {
        auto tdomain = std::get<0>(std::make_tuple(x...));
        for(auto it = tdomain.begin() ; it != tdomain.end() ; it++)
          contract((x(it)->codomain())...);
      }

      virtual std::shared_ptr<CtcBase<X...>> copy() const = 0;

    protected:

      const Index _n;
  };

  template<typename C,typename... X>
  class Ctc : public CtcBase<X...>
  {
    public:
    
      Ctc(Index n)
        : CtcBase<X...>(n)
      { }

      virtual std::shared_ptr<CtcBase<X...>> copy() const
      {
        return std::make_shared<C>(*dynamic_cast<const C*>(this));
      }
  };

  template<class C,class... X>
  concept IsCtcBaseOrPtr = (std::is_base_of_v<CtcBase<X...>,C>
      || std::is_same_v<std::shared_ptr<CtcBase<X...>>,C>);

  template<class C,class... X>
  concept IsCtcBase = std::is_base_of_v<CtcBase<X...>,C>;
  

  template<typename C>
    requires (IsCtcBase<C,Interval>) || (IsCtcBase<C,IntervalVector>)
  struct is_interval_based<C> : std::false_type {};

  template<typename C>
    requires (IsCtcBase<C,Interval>) || (IsCtcBase<C,IntervalVector>)
  struct is_ctc<C> : std::true_type {};

  template<typename C>
    requires (IsCtcBase<C,Interval>) || (IsCtcBase<C,IntervalVector>)
  struct is_sep<C> : std::false_type {};
  
  template<typename C>
    requires IsCtcBaseOrPtr<C,IntervalVector>
  const CtcBase<IntervalVector>& ctc(const C& c)
  {
    if constexpr(std::is_base_of_v<CtcBase<IntervalVector>,C>)
      return c;
    else
      return *c;
  }
}