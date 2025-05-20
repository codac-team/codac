/** 
 *  codac2_Parallelepiped.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Parallelepiped.h"
#include <iostream>
using namespace std;
using namespace codac2;

Parallelepiped::Parallelepiped(const Vector& z_, const Matrix& A_)
  : z(z_), A(A_)
{
  assert(z.size() == A.rows());
}

void generate_vertices(int i, int n, const Vector& z, const Matrix& A, vector<Vector>& L_v)
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

vector<Vector> Parallelepiped::vertices() const
{
  vector<Vector> L_v;
  generate_vertices(0, z.size(),z,A,L_v);

  return L_v;
}

