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

using namespace std;

namespace codac2
{
  /**
   * \class Zonotope
   * \brief Class representing a zonotope \f$\mathbf{z} + \mathbf{A}\cdot[-1,1]^m\f$
   * 
   * This class represents a zonotope in n-dimensional space, defined by a center point \f$\mathbf{z}\f$ and a shape matrix \f$\mathbf{A}\f$.
   * 
   * The vector \f$\mathbf{z}\f$ and each column of the matrix \f$\mathbf{A}\f$ must have the same dimension \f$n\f$, but the matrix \f$\mathbf{A}\f$ can have any number of columns \f$m\f$.
   */
  class Zonotope
  {
    public:

      /**
       * \brief Constructs a n-zonotope object with a given center and shape matrix
       * 
       * \param z Center of the zonotope (n-dimensional vector)
       * \param A Shape matrix of the zonotope (\f$n\times m\f$ matrix)
       */
      Zonotope(const Vector& z, const Matrix& A);

      /**
       * \brief Center of the zonotope
       */
      Vector z;

      /**
       * \brief Shape matrix of the zonotope
       */
      Matrix A;
      
  };
}
