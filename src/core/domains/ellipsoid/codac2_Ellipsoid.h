/** 
 *  \file codac2_Ellipsoid.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Matrix.h"
#include "codac2_Vector.h"

namespace codac2
{
  class Ellipsoid
  {
    public:

      Ellipsoid(size_t n);
      Ellipsoid(const Vector& mu_, const Matrix& Q_);

      size_t size() const;

    public:

      Vector mu;
      Matrix Q;
  };


  Ellipsoid operator+(const Ellipsoid& e1, const Ellipsoid& e2);
}