/** 
 *  \file codac2_CtcPolar.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoit Desrochers, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  class CtcPolar : public Ctc<CtcPolar,IntervalVector>
  {
    public:

      CtcPolar()
        : Ctc<CtcPolar,IntervalVector>(4)
      { }

      void contract(IntervalVector& x) const;
  };
}