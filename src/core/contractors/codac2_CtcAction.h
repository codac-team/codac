/** 
 *  \file codac2_CtcAction.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_OctaSym.h"
#include "codac2_Ctc.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class CtcAction : public Ctc<CtcAction,IntervalVector>
  {
    public:

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcAction(const C& c, const OctaSym& a)
        : Ctc<CtcAction,IntervalVector>(a.size()), _ctc(c), _s(a), __s(a.invert())
      {
        assert_release(size_of(c) == a.size());
      }

      void contract(IntervalVector& x) const;

    protected:

      const Collection<CtcBase<IntervalVector>> _ctc;
      const OctaSym _s, __s;
  };
  
  template<typename C>
    requires IsCtcBaseOrPtr<C,IntervalVector>
  inline CtcAction OctaSym::operator()(const C& c) const
  {
    return CtcAction(c, *this);
  }
}