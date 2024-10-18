/** 
 *  \file codac2_CtcLazy.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
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
  class CtcLazy : public Ctc<CtcLazy,IntervalVector>
  {
    public:

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcLazy(const C& c)
        : Ctc<CtcLazy,IntervalVector>(size_of(c)), _ctc(c),
          _r(1./std::pow(2,size_of(c)-1))
      { }

      void contract(IntervalVector& x) const;

    protected:

      const Collection<CtcBase<IntervalVector>> _ctc;
      const double _r;
  };
}