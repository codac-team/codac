/** 
 *  codac2_Zonotope.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Zonotope.h"

using namespace std;
using namespace codac2;

Zonotope::Zonotope(const Vector& z_, const Matrix& A_)
  : z(z_), A(A_)
{
  assert_release(z.size() == A.rows());
}

