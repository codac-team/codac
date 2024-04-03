/** 
 *  \file
 *  CtcPolar class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  class CtcPolar : public Ctc_<IntervalVector>
  {
    public:

      CtcPolar()
        : Ctc_<IntervalVector>(4)
      { }

      virtual std::shared_ptr<Ctc> copy() const;
      void contract(IntervalVector& x) const;
  };
}