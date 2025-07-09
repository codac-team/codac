/** 
 *  \file codac2_Parallelepiped.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <codac2_Vector.h>
#include <codac2_Matrix.h>
#include <codac2_IntervalVector.h>
#include "codac2_Zonotope.h"

using namespace std;

namespace codac2
{
  class Parallelepiped
  {
    public:
      Parallelepiped(const Vector& z, const Matrix& A);

      Vector z;
      Matrix A;

      Zonotope project(const vector<int>& indices) const;
      
      vector<Vector> vertices() const;
      IntervalVector bounding_box() const;

  };
}
