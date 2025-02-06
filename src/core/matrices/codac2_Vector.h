/** 
 *  \file codac2_Vector.h
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
  using Vector = Eigen::Matrix<double,-1,1>;

  inline std::ostream& operator<<(std::ostream& os, const Vector& x)
  {
    os << x.format(codac_vector_fmt());
    return os;
  }
}