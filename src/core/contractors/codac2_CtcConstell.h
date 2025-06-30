/** 
 *  \file codac2_CtcConstell.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  /**
   * \class CtcConstell
   */
  class CtcConstell : public Ctc<CtcConstell,IntervalVector>
  {
    public:

      /**
       * \brief Creates the contractor.
       */
      CtcConstell(const std::vector<IntervalVector>& M);

      void contract(IntervalVector& x) const;

    protected:

      const std::vector<IntervalVector> _M;
  };
}