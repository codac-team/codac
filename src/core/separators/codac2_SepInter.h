/** 
 *  \file codac2_SepInter.h
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
  class SepInter : public Sep<SepInter>
  {
    public:

      template<typename S>
        requires (IsSepBaseOrPtr<S> && !std::is_same_v<SepInter,S>)
      SepInter(const S& s)
        : Sep<SepInter>(size_of(s)), _seps(s)
      { }

      template<typename... S>
        requires (IsSepBaseOrPtr<S> && ...)
      SepInter(const S&... s)
        : Sep<SepInter>(size_first_item(s...)), _seps(s...)
      {
        assert_release(all_same_size(s...));
      }

      BoxPair separate(const IntervalVector& x) const;

      template<typename S>
        requires std::is_base_of_v<SepBase,S>
      SepInter& operator&=(const S& s)
      {
        assert_release(s.size() == this->size());
        _seps.push_object_back(s);
        return *this;
      }

      SepInter& operator&=(const std::shared_ptr<SepBase>& s)
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
  inline SepInter operator&(const S1& s1, const S2& s2)
  {
    return SepInter(s1,s2);
  }

  template<typename S2>
    requires IsSepBaseOrPtr<S2>
  inline SepInter operator&(const IntervalVector& s1, const S2& s2)
  {
    assert_release(s1.size() == s2.size());
    return SepInter(SepWrapper(s1),s2);
  }

  template<typename S1>
    requires IsSepBaseOrPtr<S1>
  inline SepInter operator&(const S1& s1, const IntervalVector& s2)
  {
    assert_release(s1.size() == s2.size());
    return SepInter(s1,SepWrapper(s2));
  }
}