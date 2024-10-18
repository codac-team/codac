/** 
 *  \file codac2_Ellipsoid.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louedec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Matrix.h"
#include "codac2_Vector.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_BoolInterval.h"

namespace codac2
{
  class Ellipsoid
  {
    public:

      Ellipsoid(size_t n);

      Ellipsoid(const Vector &mu_, const Matrix &G_);

      size_t size() const;

      /**
       * \brief Returns a random vector inside this ellipsoid
       *
       * \note The seed of the pseudo-random number generator is 
       *       voluntarily initialized outside this function, on demand.
       *
       * \return random value
       */
      Vector rand() const;
      
      /**
       * \brief Compute the tightest axis-aligned box containing this ellipsoid
       *
       * \return hull box
       */
      IntervalVector hull_box() const;


      /**
       * \brief Compute if the provided ellipsoid is a subset of this ellipsoid,
       *        provided that the two ellipsoids share the same center
       *
       * \param e the second ellipsoid of same dimension
       * \return a BoolInterval
       */
      BoolInterval is_concentric_subset(const Ellipsoid& e) const;

    public:

      Vector mu;
      Matrix G;
  };


    Ellipsoid operator+(const Ellipsoid &e1, const Ellipsoid &e2);

    // return A*e+b | non-rigorous operations
    Ellipsoid unreliable_linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b);

    // compute en outer enclosure of A*e+b, considering the rounding error with pessimism
    Ellipsoid linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b);

        /* nonsingular main case from
         * Rauh, A., & Jaulin, L. (2021).
         * "A computationally inexpensive algorithm for determining outer
         * and inner enclosures of nonlinear mappings of ellipsoidal domains.
         * International Journal of Applied Mathematics and Computer Science, 31(3).*/

    Ellipsoid nonlinear_mapping_base(const Ellipsoid &e, const Matrix &J, const IntervalMatrix &J_box,
                                     const Vector& trig1,
                                     const Vector& q);

    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorOpValue> &f);

    // compute an outer ellipsoidal enclosure of f(e)
    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorOpValue> &f,
                                const Vector& trig,const Vector& q);



    std::ostream &operator<<(std::ostream &os, const Ellipsoid &e);

}