/** 
 *  \file
 *  SepAction class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_CollectionSep.h"
#include "codac2_OctaSym.h"

namespace codac2
{
  class SepAction : public CollectionSep, public Sep
  {
    public:

      template<typename S, // S should be some Sep class
        typename = typename std::enable_if<std::is_base_of<Sep,S>::value>::type>
      SepAction(const S& s, const OctaSym& a)
        : CollectionSep(s), _s(a), __s(a.invert())
      { }

      SepAction(std::shared_ptr<Sep> s, const OctaSym& a)
        : CollectionSep(), _s(a), __s(a.invert())
      {
        add_shared_ptr(s);
      }

      virtual std::shared_ptr<Sep> copy() const;
      virtual BoxPair separate(const IntervalVector& x) const;

    protected:

      const OctaSym _s, __s;
  };
  
  template<typename S, // S should be some Sep class
    typename = typename std::enable_if<std::is_base_of<Sep,S>::value>::type>
  inline SepAction OctaSym::operator()(const S& s) const
  {
    return SepAction(s, *this);
  }
}