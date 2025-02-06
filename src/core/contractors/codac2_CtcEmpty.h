/** 
 *  \file codac2_CtcEmpty.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_CtcWrapper.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  class CtcEmpty : public Ctc<CtcEmpty,IntervalVector>
  {
    public:

      CtcEmpty(Index n);

      void contract(IntervalVector& x) const;
  };
}