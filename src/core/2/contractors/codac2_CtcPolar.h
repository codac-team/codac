/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCPOLAR__
#define __CODAC2_CTCPOLAR__

#include "codac2_Ctc.h"
#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  /**
   * \class CtcPolar
   */
  class CtcPolar : public Ctc
  {
    public:

      CtcPolar();
      void contract(IntervalVector& x);
      void contract(IntervalVector& xy, IntervalVector& rhotheta);
      void contract(Interval& x, Interval& y, Interval& rho, Interval& theta);

      make_available_to_cn()
  };
}

#endif