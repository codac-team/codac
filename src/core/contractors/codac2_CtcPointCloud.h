/** 
 *  \file codac2_CtcPointCloud.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  class CtcPointCloud : public Ctc<CtcPointCloud,IntervalVector>
  {
    public:

      CtcPointCloud(const std::vector<IntervalVector>& p);
      void contract(IntervalVector& x) const;

    protected:

      const std::vector<IntervalVector> _p;
  };
}