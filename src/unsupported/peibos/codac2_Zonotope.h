/** 
 *  \file codac2_Zonotope.h
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

using namespace std;

namespace codac2
{
  class Zonotope
  {
    public:
      Zonotope(const Vector& z, const Matrix& A);

      Vector z;
      Matrix A;
      
  };
}
