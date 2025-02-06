/** 
 *  \file codac2_Matrix.h
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
  using Matrix = Eigen::Matrix<double,-1,-1>;

  inline std::ostream& operator<<(std::ostream& os, const Matrix& x)
  {
    os << x.format(codac_matrix_fmt());
    return os;
  }
}