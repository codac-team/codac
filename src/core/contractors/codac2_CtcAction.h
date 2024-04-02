/** 
 *  \file
 *  CtcAction class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_OctaSym.h"
#include "codac2_Ctc.h"
#include "codac2_Collection.h"

namespace codac2
{
  class CtcAction : public Ctc
  {
    public:

      template<typename C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<IntervalVector>,C>
        >::type>
      CtcAction(const C& c, const OctaSym& a)
        : _ctc(c), _s(a), __s(a.invert())
      { }

      virtual std::shared_ptr<Ctc> copy() const;
      void contract(IntervalVector& x) const;

    protected:

      const Collection<Ctc_<IntervalVector>> _ctc;
      const OctaSym _s, __s;
  };
  
  template<typename C, typename = typename std::enable_if<
      std::is_base_of_v<Ctc_<IntervalVector>,C>
    >::type>
  inline CtcAction OctaSym::operator()(const C& c) const
  {
    return CtcAction(c, *this);
  }
}