/** 
 *  \file codac2_Ellipsoid.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louédec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Matrix.h"
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_BoolInterval.h"

namespace codac2
{
  /**
   * \brief Ellipsoid representation
   * 
   * References:
   *
   *   Concentric subset test and degenerate computations are inspired by the
   *   work of Morgan Louedec. Concepts and algorithms are provided in:
   *
   *     Stability analysis of the formation control of a group of
   *     underwater robots, with ellipsoidal enclosure.
   *     Morgan Louédec, 2024
   *     PhD thesis, Université de Bretagne Occidentale, Brest, France.
   *     https://morgan-louedec.fr/thesis-of-morgan-louedec
   *
   *   Non-linear mapping of the nonsingular main case is inspired by:
   *
   *     A computationally inexpensive algorithm for determining outer
   *     and inner enclosures of nonlinear mappings of ellipsoidal domains.
   *     Andreas Rauh, Luc Jaulin
   *     2021, International Journal of Applied Mathematics and Computer Science, 
   *     doi: https://doi.org/10.34768/amcs-2021-0027
   */
  class Ellipsoid
  {
    public:

      /**
       * \brief Create a n-dimensional ellipsoid
       * 
       * \param n number of dimensions
       */
      Ellipsoid(Index n);

      /**
       * \brief Create an ellipsoid from a center and a shape matrix
       * 
       * \param mu midpoint vector
       * \param G shape matrix
       */
      Ellipsoid(const Vector& mu, const Matrix& G);

      /**
       * \brief Return the dimension of the ellipsoid
       *
       * \return number of dimensions
       */
      Index size() const;

      /**
       * \brief Return a random vector inside this ellipsoid
       *
       * \note The seed of the pseudo-random number generator is 
       *       voluntarily initialized outside this function, on demand.
       *
       * \return random vector value
       */
      Vector rand() const;

      /**
       * \brief Compute the tightest axis-aligned box containing this ellipsoid
       *
       * \return hull box
       */
      IntervalVector hull_box() const;

      /**
       * \brief Test if the provided ellipsoid is a subset of this ellipsoid,
       *        provided that the two ellipsoids share the same center
       *
       * \param e the second ellipsoid of same dimension
       * \return a BoolInterval
       */
      BoolInterval is_concentric_subset(const Ellipsoid& e) const;

      /**
       * \brief Project the ellipsoid on the 2d plane (d,v,u)
       * \param d a point on the plane
       * \param v a vector of the plane
       * \param u a other vector of the plane, orthogonal to v
       * \return the projected 2d ellipsoid
       */
      Ellipsoid proj_2d(const Vector& d, const Vector& v, const Vector& u) const;

    public:

      Vector mu; ///< midpoint vector
      Matrix G;  ///< shape matrix
  };

  /**
   * \brief Compute the Minkowski sum of two ellipsoids
   *
   * \param e1 first ellipsoid
   * \param e2 second ellipsoid
   * \return the Minkowski sum
   */
  Ellipsoid operator+(const Ellipsoid& e1, const Ellipsoid& e2);

  /**
   * \brief Guaranteed linear evaluation A*e+b,
   *        considering the rounding errors
   *
   * \param e input ellipsoid
   * \param A matrix
   * \param b vector
   * \return a rigorous outer enclosure of the linear mapping
   */
  Ellipsoid linear_mapping(const Ellipsoid& e, const Matrix& A, const Vector& b);

  /**
   * \brief Nonrigorous linear evaluation A*e+b
   * 
   * \note This function is used in linear_mapping() and provides
   *       a faster output than its guaranteed counterpart.
   *
   * \param e input ellipsoid
   * \param A matrix
   * \param b vector
   * \return a nonrigorous approximation of the linear mapping
   */
  Ellipsoid unreliable_linear_mapping(const Ellipsoid& e, const Matrix& A, const Vector& b);

  /**
   * \brief (Rigorous?) non-linear evaluation f(e)
   *
   * \param e input ellipsoid
   * \param f non-linear analytical function
   * \return a (rigorous?) outer enclosure of the non-linear mapping
   */
  Ellipsoid nonlinear_mapping(const Ellipsoid& e, const AnalyticFunction<VectorType>& f);

  /**
   * \brief (Rigorous?) non-linear evaluation f(e), with parameters
   *
   * \param e input ellipsoid
   * \param f non-linear analytical function
   * \param trig (?)
   * \param q (?)
   * \return a (rigorous?) outer enclosure of the non-linear mapping
   */
  Ellipsoid nonlinear_mapping(const Ellipsoid& e, const AnalyticFunction<VectorType>& f,
                              const Vector& trig, const Vector& q);

  /**
   * \brief (Rigorous?) non-linear evaluation f(e), from Jacobian information
   *
   * \param G (?)
   * \param J approximated Jacobian matrix of f
   * \param J_box reliable enclosure of the Jacobian matrix of f
   * \param trig (?)
   * \param q (?)
   * \return a (rigorous?) outer enclosure of the non-linear mapping
   */
  Matrix nonlinear_mapping_base(const Matrix& G, const Matrix& J, const IntervalMatrix& J_box,
                                const Vector& trig, const Vector& q);

  /**
   * \brief Streams out an Ellipsoid
   * 
   * \param os the stream to be updated
   * \param e the ellipsoid stream out
   * \return a reference to the updated stream
   */
  std::ostream& operator<<(std::ostream& os, const Ellipsoid& e);
}