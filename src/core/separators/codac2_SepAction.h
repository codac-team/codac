/** 
 *  \file codac2_SepAction.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_OctaSym.h"

namespace codac2
{
  class SepAction : public Sep<SepAction>
  {
    public:

      template<typename S>
        requires IsSepBaseOrPtr<S>
      SepAction(const S& s, const OctaSym& a)
        : Sep<SepAction>(a.size()), _sep(s), _s(a), __s(a.invert())
      {
        assert_release(size_of(s) == (Index)a.size());
      }

      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<SepBase> _sep;
      const OctaSym _s, __s;
  };
  
  template<typename S>
    requires is_sep_v<S>
  inline SepAction OctaSym::operator()(const S& s) const
  {
    return SepAction(s, *this);
  }
}