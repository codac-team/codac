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
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  /**
   * \class Zonotope
   * \brief Class representing a zonotope \f$\mathbf{c} + \mathbf{A}\cdot[-1,1]^m\f$
   * 
   * This class represents a zonotope in n-dimensional space, defined by a center point \f$\mathbf{c}\f$ and a shape matrix \f$\mathbf{A}\f$.
   * 
   * The vector \f$\mathbf{c}\f$ and each column of the matrix \f$\mathbf{A}\f$ must have the same dimension \f$n\f$, but the matrix \f$\mathbf{A}\f$ can have any number of columns \f$m\f$.
   */
  class Zonotope
  {
    public:

      /**
       * \brief Constructs a n-zonotope object with a given center and shape matrix
       * 
       * \param c Center of the zonotope (n-dimensional vector)
       * \param A Shape matrix of the zonotope (\f$n\times m\f$ matrix)
       */
      Zonotope(const Vector& c, const Matrix& A);

        /**
        * \brief Constructs an empty n-zonotope
        * 
        * \param n Dimension of the zonotope
        * 
        * \return A new Zonotope object representing an empty zonotope in n-dimensional space
        */
      static Zonotope empty(Index n);

      /**
       * \brief Checks if the zonotope is empty
       * 
       * \return True if the zonotope is empty, false otherwise
       */
      bool is_empty() const;

      /**
       * \brief Sets the zonotope to be empty
       */
      void set_empty();

      /**
       * \brief Outputs the size of the zonotope
       * 
       * \return The size of the zonotope, defined as the size of its center vector
       */

      Index size() const { return c.size(); }

      /**
       * \brief Computes the axis-aligned bounding box of the zonotope
       * 
       * \return An IntervalVector representing the axis-aligned bounding box of the zonotope
       */
      IntervalVector box() const;

      /**
       * \brief Projects the Zonotope onto the subspace defined by the given indices
       * 
       * \param indices Vector of indices of the dimensions to project onto
       * 
       * \return A new Zonotope object representing the projection of the Zonotope onto the specified subspace
       */
      Zonotope proj(const std::vector<Index>& indices) const;

      /**
       * \brief Computes the Minkowski sum of this Zonotope with another Zonotope
       * 
       * \param zonotope The other Zonotope to add to this one
       * 
       * \return A new Zonotope object representing the Minkowski sum of the two Zonotopes
       */
      Zonotope operator+(const Zonotope& zonotope) const;

      /** 
       * \brief Create a new zonotope englobing (*this) with at most
       * \f$n\times o\f$ columns (with \f$n\f$ the dimension of the zonotope.
       * Return a copy if the number of columns is already
       * less than or equal to \f$n\times o\f$. Note that \f$o\f$ must be 
       * at least 1 (returns a parallelotope if \f$o=1\f$).
       * The method is rather coarse and use a full pivot
       * LU decomposition to determine the columns which are kept rather than
       * more complex computations).
       *
       * \param o the new maximum order (i.e. \f$m/n\f$) of the zonotope.
       * \param invert if true, the zonotope created is at least
       * a parallelotope (i.e. the matrix is full rank).
       * \return a new zonotope.
       */ 
      Zonotope reduction(double o, bool invert=false) const;

      /**
       * \brief Center of the zonotope
       */
      Vector c;

      /**
       * \brief Shape matrix of the zonotope
       */
      Matrix A;

      /**
       * \brief Flag indicating whether the zonotope is empty
       */
      bool empty_flag = false;
  };

  /**
   * \brief Stream output operator for ``Zonotope``.
   *
   * \param str Output stream.
   * \param z The zonotope to print.
   * \return The output stream with the zonotope information.
   */
  std::ostream& operator<<(std::ostream& str, const Zonotope& z);
}
