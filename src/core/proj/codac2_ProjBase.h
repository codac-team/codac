/** 
 *  \file codac2_CtcProj.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_IntervalVector.h"

namespace codac2
{
  class ProjBase
  {
    protected:

      ProjBase(const std::vector<Index>& proj_indices, const IntervalVector& y, double default_eps = 0.01);
      IntervalVector extract_x(const IntervalVector& w) const;
      IntervalVector extract_y(const IntervalVector& w) const;
      IntervalVector cart_prod_xy(const IntervalVector& x, const IntervalVector& y) const;
      Index y_max_diam_index(const IntervalVector& y) const;

    protected:

      const Index _n;
      const std::vector<Index> _xi;
      const IntervalVector _y;
      const double _default_eps;
  };
}