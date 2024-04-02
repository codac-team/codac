/** 
 *  \file
 *  SepUnion class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <memory>
#include "codac2_Sep.h"
#include "codac2_CollectionSep.h"
#include "codac2_SepWrapper.h"

namespace codac2
{
  class SepUnion : public CollectionSep, public Sep
  {
    public:

      template<typename... S>
      SepUnion(const S&... s)
        : CollectionSep(s...)
      { }

      //explicit SepUnion(const SepUnion& s)
      //  : CollectionSep(s)
      //{ }

      virtual std::shared_ptr<Sep> copy() const;
      BoxPair separate(const IntervalVector& x) const;
  };

  template<typename S1, // S1 should be some Sep class
    typename = typename std::enable_if<std::is_base_of<Sep,S1>::value>::type,
           typename S2, // S2 should be some Sep class
    typename = typename std::enable_if<std::is_base_of<Sep,S2>::value>::type>
  inline SepUnion operator|(const S1& s1, const S2& s2)
  {
    return SepUnion(s1,s2);
  }

  template<typename S2, // S2 should be some Sep class
    typename = typename std::enable_if<std::is_base_of<Sep,S2>::value>::type>
  inline SepUnion operator|(const IntervalVector& s1, const S2& s2)
  {
    return SepUnion(SepWrapper_<IntervalVector>(s1),s2);
  }

  template<typename S1, // S1 should be some Sep class
    typename = typename std::enable_if<std::is_base_of<Sep,S1>::value>::type>
  inline SepUnion operator|(const S1& s1, const IntervalVector& s2)
  {
    return SepUnion(s1,SepWrapper_<IntervalVector>(s2));
  }
}