/** 
 *  \file codac2_IntervalRow.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_matrices.h"

namespace codac2
{
  using IntervalRow = Eigen::Matrix<Interval,1,-1>;

  inline std::ostream& operator<<(std::ostream& os, const IntervalRow& x)
  {
    if(x.is_empty())
      return os << "[ empty row ]";

    else
    {
      os << x.format(codac_row_fmt());
      return os;
    }
  }
}