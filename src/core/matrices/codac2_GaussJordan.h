/** 
 *  \file codac2_GaussJordan.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>
#include "codac2_eigen.h"
#include "codac2_Matrix.h"

namespace codac2
{
  // Gauss Jordan band diagonalization preconditioning
  Matrix gauss_jordan(const Matrix& A);
}