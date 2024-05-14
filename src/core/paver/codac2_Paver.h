/** 
 *  \file
 *  Paver class
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
#include "codac2_WithGraphicOutput.h"

namespace codac2
{
  class Paver : public WithGraphicOutput
  {
    public:

      Paver(const IntervalVector& x0);
      void pave(const Ctc_<IntervalVector>& c, double eps);

    protected:

      const IntervalVector _x0;
  };
}