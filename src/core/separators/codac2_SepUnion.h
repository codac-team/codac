/** 
 *  \file codac2_SepUnion.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
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
  class SepUnion : public Sep<SepUnion>
  {
    public:

      template<typename S>
        requires (IsSepBaseOrPtr<S> && !std::is_same_v<SepUnion,S>)
      SepUnion(const S& s)
        : Sep<SepUnion>(size_of(s)), _seps(s)
      { }

      template<typename... S>
        requires (IsSepBaseOrPtr<S> && ...)
      SepUnion(const S&... s)
        : Sep<SepUnion>(size_first_item(s...)), _seps(s...)
      {
        assert_release(all_same_size(s...));
      }

      BoxPair separate(const IntervalVector& x) const;

      template<typename S>
        requires std::is_base_of_v<SepBase,S>
      SepUnion& operator|=(const S& s)
      {
        assert_release(s.size() == this->size());
        _seps.push_back(s);
        return *this;
      }

      SepUnion& operator|=(const std::shared_ptr<SepBase>& s)
      {
        assert_release(s->size() == this->size());
        _seps.push_back(s);
        return *this;
      }

    protected:

      Collection<SepBase> _seps;
  };

  template<typename S1, typename S2>
    requires (IsSepBaseOrPtr<S1> && IsSepBaseOrPtr<S2>)
  inline SepUnion operator|(const S1& s1, const S2& s2)
  {
    return SepUnion(s1,s2);
  }

  template<typename S2>
    requires IsSepBaseOrPtr<S2>
  inline SepUnion operator|(const IntervalVector& s1, const S2& s2)
  {
    assert_release(s1.size() == s2.size());
    return SepUnion(SepWrapper(s1),s2);
  }

  template<typename S1>
    requires IsSepBaseOrPtr<S1>
  inline SepUnion operator|(const S1& s1, const IntervalVector& s2)
  {
    assert_release(s1.size() == s2.size());
    return SepUnion(s1,SepWrapper(s2));
  }
}