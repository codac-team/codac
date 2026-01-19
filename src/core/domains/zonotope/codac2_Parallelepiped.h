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
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_Zonotope.h"
#include "codac2_BoolInterval.h"

namespace codac2
{
  /**
   * \class Parallelepiped
   * 
   * \brief Class representing a parallelepiped \f$\mathbf{z} + \mathbf{A}\cdot[-1,1]^m\f$
   * 
   * This class represents a parallelepiped in n-dimensional space, defined by a center point \f$\mathbf{z}\f$ and a shape matrix \f$\mathbf{A}\f$.
   * 
   * A parallelepiped is a special case of a zonotope where the shape matrix \f$\mathbf{A}\f$ has \f$m\f$ columns with \f$m \leqslant n\f$.
   */
  class Parallelepiped : public Zonotope
  {
    public:

      /**
       * \brief Constructs a n-parallelepiped object with a given center and shape matrix
       * 
       * \param z Center of the parallelepiped (n-dimensional vector)
       * \param A Shape matrix of the parallelepiped (\f$n\times m\f$ matrix with \f$m \leqslant n\f$)
       */
      Parallelepiped(const Vector& z, const Matrix& A);
      
      /**
       * \brief Computes the vertices of the parallelepiped
       * 
       * \return A vector containing the **unsorted** vertices of the parallelepiped
       */
      std::vector<Vector> vertices() const;

      /**
       * \brief Checks if a given point is contained within the parallelepiped. The matrix A has to be square and invertible.
       * 
       * \param v The point to check
       * 
       * \return BoolInterval true if the point is inside the parallelepiped, false if is outside, and unknown otherwise
       */
      BoolInterval contains(const Vector& v) const;

      /**
       * \brief Checks if a given box is contained within the parallelepiped. The matrix A has to be square and invertible.
       * 
       * \param x The box to check
       * 
       * \return BoolInterval true if the box is inside the parallelepiped, false if is outside, and unknown otherwise
       */
      BoolInterval is_superset(const IntervalVector& x) const;

  };
}
