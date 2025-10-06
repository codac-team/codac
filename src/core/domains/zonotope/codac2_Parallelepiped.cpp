/** 
 *  codac2_Parallelepiped.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Parallelepiped.h"

using namespace std;
using namespace codac2;

Parallelepiped::Parallelepiped(const Vector& z_, const Matrix& A_)
  : Zonotope(z_, A_)
{
  assert_release(A.cols() <= z.size() && "too many vectors, you are describing a zonotope");
}

Zonotope Parallelepiped::proj(const std::vector<Index>& indices) const
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

void generate_vertices(Index i, Index n, const Vector& z, const Matrix& A, vector<Vector>& L_v)
{
  if (i == n)
  {
    L_v.push_back(z);
  }
  else if (i<n)
  {
    generate_vertices(i+1, n, z + A.col(i), A, L_v);
    generate_vertices(i+1, n, z - A.col(i), A, L_v);
  }
}

std::vector<Vector> Parallelepiped::vertices() const
{
  vector<Vector> L_v;
  generate_vertices(0, z.size(),z,A,L_v);
  return L_v;
}

IntervalVector Parallelepiped::box() const
{
  IntervalVector box(z);
  for(const auto& v : vertices())
    box |= v;
  return box;
}