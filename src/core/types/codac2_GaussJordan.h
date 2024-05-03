/** 
 *  \file
 *  GaussJordan functions
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <ostream>
#include "codac2_eigen.h"
#include "codac2_Matrix.h"

namespace codac2
{
  Matrix gauss_jordan(const Matrix& A);
}