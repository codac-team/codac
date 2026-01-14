/** 
 *  codac2_Zonotope.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Zonotope.h"

using namespace codac2;

Zonotope::Zonotope(const Vector& z_, const Matrix& A_)
  : z(z_), A(A_)
{
  assert_release(z.size() == A.rows());
}

Zonotope Zonotope::proj(const std::vector<Index>& indices) const
{
  assert_release(*std::min_element(indices.begin(), indices.end()) >= 0 && "indices out of range");
  assert_release(*std::max_element(indices.begin(), indices.end()) <= z.size() && "indices out of range");

  Matrix A_cropped (indices.size(), A.cols());
  Vector z_cropped (indices.size());

  for (size_t i = 0; i < indices.size(); ++i)
  {
    A_cropped.row(i) = A.row(indices[i]);
    z_cropped[i] = z[indices[i]];
  }

  return Zonotope(z_cropped, A_cropped);
}

