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
  : z(z_), A(A_)
{
  assert(z.size() == A.rows());
}

vector<Vector> Parallelepiped::vertices() const
{
  assert(A.rows() == 3);
  vector<Vector> vertices;
  Vector v1 = z + A.col(0) + A.col(1) + A.col(2);
  Vector v2 = z + A.col(0) - A.col(1) + A.col(2);
  Vector v3 = z - A.col(0) - A.col(1) + A.col(2);
  Vector v4 = z - A.col(0) + A.col(1) + A.col(2);
  Vector v5 = z - A.col(0) + A.col(1) - A.col(2);
  Vector v6 = z + A.col(0) + A.col(1) - A.col(2);
  Vector v7 = z + A.col(0) - A.col(1) - A.col(2);
  Vector v8 = z - A.col(0) - A.col(1) - A.col(2);
  vertices.push_back(v1);
  vertices.push_back(v2);
  vertices.push_back(v3);
  vertices.push_back(v4);
  vertices.push_back(v5);
  vertices.push_back(v6);
  vertices.push_back(v7);
  vertices.push_back(v8);

  return vertices;
}
