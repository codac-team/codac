/** 
 *  \file codac2_IntervalMatrix.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_matrices.h"
#include "codac2_Vector.h"

namespace codac2
{
  using IntervalMatrix = Eigen::Matrix<Interval,-1,-1>;

  inline std::ostream& operator<<(std::ostream& os, const IntervalMatrix& x)
  {
    if(x.is_empty())
      return os << "[ empty " << x.rows() << "x" << x.cols() << " mat ]";

    else
    {
      os << x.format(codac_matrix_fmt());
      return os;
    }
  }
}