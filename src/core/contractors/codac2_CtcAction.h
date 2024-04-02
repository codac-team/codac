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
#include "codac2_CtcWrapper.h"
#include "codac2_CollectionCtc.h"
#include "codac2_IntervalVector.h"
#include "codac2_OctaSym.h"

namespace codac2
{
  class CtcAction : public CollectionCtc<IntervalVector>
  {
    public:

      CtcAction(const Ctc_<IntervalVector>& c, const OctaSym& a)
        : CollectionCtc<IntervalVector>(), _s(a), __s(a.invert())
      {
        add_shared_ptr(std::dynamic_pointer_cast<Ctc_<IntervalVector>>(c.copy()));
      }

      virtual std::shared_ptr<Ctc> copy() const;
      void contract(IntervalVector& x) const;

    protected:

      const OctaSym _s, __s;
  };
}