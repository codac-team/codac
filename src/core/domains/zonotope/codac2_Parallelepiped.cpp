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

bool Parallelepiped::contains(const Vector& v) const
{
  assert_release(A.rows() == A.cols() && "Matrix A must be square to check containment.");

  IntervalVector IV = Interval(-1,1)*IntervalVector::Ones(A.cols());

  return IV.contains(A.inverse()*(v - z));
}