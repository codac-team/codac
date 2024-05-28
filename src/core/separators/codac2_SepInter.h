/** 
 *  \file
 *  SepInter class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_SepWrapper.h"
#include "codac2_templates.h"

namespace codac2
{
  class SepInter : public Sep
  {
    public:

      template<typename S, typename = typename std::enable_if<(
          (std::is_base_of_v<Sep,S> && !std::is_same_v<SepInter,S>) || std::is_same_v<std::shared_ptr<Sep>,S>
        ), void>::type>
      SepInter(const S& s)
        : Sep(size_of(s)), _seps(s)
      { }

      template<typename... S, typename = typename std::enable_if<(true && ... && (
          (std::is_base_of_v<Sep,S> || std::is_same_v<std::shared_ptr<Sep>,S>)
        )), void>::type>
      SepInter(const S&... s)
        : Sep(size_first_item(s...)), _seps(s...)
      {
        assert(all_same_size(s...));
      }

      virtual std::shared_ptr<Sep> copy() const;
      BoxPair separate(const IntervalVector& x) const;

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S>
        >::type>
      SepInter& operator&=(const S& s)
      {
        _seps.add_shared_ptr(std::make_shared<S>(s));
        return *this;
      }

    protected:

      Collection<Sep> _seps;
  };

  template<typename S1, typename S2, typename = typename std::enable_if<(
      std::is_base_of_v<Sep,S1> &&
      std::is_base_of_v<Sep,S2>
    )>::type>
  inline SepInter operator&(const S1& s1, const S2& s2)
  {
    return SepInter(s1,s2);
  }

  template<typename S2, typename = typename std::enable_if<
      std::is_base_of_v<Sep,S2>
    >::type>
  inline SepInter operator&(const IntervalVector& s1, const S2& s2)
  {
    return SepInter(SepWrapper_<IntervalVector>(s1),s2);
  }

  template<typename S1, typename = typename std::enable_if<
      std::is_base_of_v<Sep,S1>
    >::type>
  inline SepInter operator&(const S1& s1, const IntervalVector& s2)
  {
    return SepInter(s1,SepWrapper_<IntervalVector>(s2));
  }
}