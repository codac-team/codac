/** 
 *  \file codac2_CtcFixpoint.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_IntervalVector.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class CtcFixpoint : public Ctc<CtcFixpoint,IntervalVector>
  {
    public:

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcFixpoint(const C& c, double ratio = 0.1)
        : Ctc<CtcFixpoint,IntervalVector>(size_of(c)), _ctc(c),
          _r(ratio)
      { }

      void contract(IntervalVector& x) const;

    protected:

      const Collection<CtcBase<IntervalVector>> _ctc;
      const double _r;
  };
}