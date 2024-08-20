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

namespace codac2
{
  class CtcAction : public Ctc_<IntervalVector>
  {
    public:

      template<typename C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<IntervalVector>,C> || std::is_same_v<std::shared_ptr<Ctc_<IntervalVector>>,C>
        >::type>
      CtcAction(const C& c, const OctaSym& a)
        : Ctc_<IntervalVector>(a.size()), _ctc(c), _s(a), __s(a.invert())
      {
        assert_release(size_of(c) == a.size());
      }

      std::shared_ptr<Ctc_<IntervalVector>> copy() const;
      void contract(IntervalVector& x) const;

    protected:

      const Collection<Ctc_<IntervalVector>> _ctc;
      const OctaSym _s, __s;
  };
  
  template<typename C, typename>
  inline CtcAction OctaSym::operator()(const C& c) const
  {
    return CtcAction(c, *this);
  }
}